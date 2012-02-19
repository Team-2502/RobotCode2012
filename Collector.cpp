#include <WPILib.h>
#include "Collector.h"

unsigned Collector::balls = 0;
Victor *Collector::grabber = NULL;
Victor *Collector::lifter = NULL;
Sharp_IR *Collector::frontIR = NULL;
Sharp_IR *Collector::middleIR = NULL;
Sharp_IR *Collector::topIR = NULL;
CollectorState Collector::collectorState = OFF;

Collector::Collector( )
{
	grabber = new Victor(COLLECTOR_GRABBER_CHANNEL);
	lifter = new Victor(COLLECTOR_LIFTER_CHANNEL);
	frontIR = new Sharp_IR(1, IR_FRONT_DIO , COLLECTOR_FRONT_SIGNAL_VOLTAGE);
	middleIR = new Sharp_IR(1, IR_MIDDLE_DIO, COLLECTOR_MIDDLE_SIGNAL_VOLTAGE);
	topIR = new Sharp_IR(1, IR_TOP_DIO , COLLECTOR_TOP_SIGNAL_VOLTAGE);
	collectorState = OFF;
	collectorTask = new Task("2502Cl",(FUNCPTR)ThreadLoop);
	strike1 = new Relay(RAMP_LEFT_SPIKE);
	strike2 = new Relay(RAMP_RIGHT_SPIKE);
}

Collector::~Collector()
{
	collectorTask->Stop();
	delete collectorTask;

	delete grabber;
	delete lifter;
	delete frontIR;
	delete middleIR;
	delete topIR;
	delete strike1;
	delete strike2;
}

void Collector::Shoot()
{
	if( collectorState == LOOKING_FOR_BALLS || collectorState == OFF )
	{
		collectorState = SHOOTING;
	}
}

void Collector::PrepareToShoot()
{
	collectorState = PREPARE_TO_SHOOT;
	// Don't wait longer than 1 second
	for( int count = 0; count < 100 ; count ++ )
	{
		if( collectorState == OFF )
			break;
		Wait( .01 );
	}
}
void Collector::ManipulateRamp(RampState state)
{
	if (collectorState == OFF || collectorState == LOOKING_FOR_BALLS)
	{
		grabber->Set(COLLECTOR_STOP);
		lifter->Set(COLLECTOR_STOP);
		if (state == UP)
		{
			strike1->Set(Relay::kForward);
			strike2->Set(Relay::kReverse);
		}
		else if (state == DOWN)
		{
			strike1->Set(Relay::kReverse);
			strike2->Set(Relay::kForward);
		}
		else
		{
			strike1->Set(Relay::kOff);
			strike2->Set(Relay::kOff);
			collectorState = LOOKING_FOR_BALLS;
		}
	}
}


void Collector::SetBallCount( int ballCount )
{
	balls = ballCount;
}

// Turn off everything
void Collector::Stop()
{
	grabber->Set(COLLECTOR_STOP);
	lifter->Set(COLLECTOR_STOP);
	collectorState = OFF;
}

// Ejects ALL Balls - as long as it is running
void Collector::Eject()
{
	grabber->Set(COLLECTOR_RUNFAST_REVERSE);
	lifter->Set(COLLECTOR_RUNFAST_REVERSE);
	Wait( 1 );
	grabber->Set(COLLECTOR_STOP);
	lifter->Set(COLLECTOR_STOP);
	balls = 0;
	collectorState = LOOKING_FOR_BALLS;
}

// If collector is not doing something else, turn it on.
void Collector::Start()
{
	if( collectorState == OFF )
	{
		collectorTask->Start();
		collectorState = LOOKING_FOR_BALLS;
	}
}

void Collector::RejectBall()
{
	grabber->Set( COLLECTOR_RUNFAST_REVERSE );
	Wait( .5 );
	grabber->Set( COLLECTOR_STOP );
}

// Thread that runs continuously
void Collector::ThreadLoop()
{
	while( true )
	{
		switch( collectorState )
		{
		case OFF:
			// Reject any balls that show up.
			if( frontIR->Get() == BALL_VISIBLE )
				RejectBall();
			Wait( .01 );
			break;
		case LOOKING_FOR_BALLS:
			// if we mount a sensor up front, we could turn on when we sense a ball if we have less than 2 balls
			if( frontIR->Get() == BALL_VISIBLE )
			{
				if( balls < MAX_BALLS )
				{
					collectorState = STAGE1;
				}
				else
				{
					// already enough balls, kick it out of our way
					RejectBall();
				}
			}
			Wait( .01 );
			break;
		case STAGE1:
			if( balls < MAX_BALLS )
			{
				grabber->Set(COLLECTOR_RUNFAST);
				if( middleIR->Get() == BALL_VISIBLE ) // Does bottom IR sense Ball?
				{
					// probably want to run this one slowly because we want to stop as soon as the IR sensor no longer senses the ball
					grabber->Set(COLLECTOR_RUNSLOW);
					lifter->Set(COLLECTOR_RUNSLOW);  
					Wait( .03 ); // enough time to transfer the ball beyond the pully
					grabber->Set(COLLECTOR_STOP);
					collectorState = STAGE2;
				}
			}
			else
			{
				// Already have enough balls, kick out any balls in the way and go back to looking which will eject any new balls that appear
				collectorState = LOOKING_FOR_BALLS;
				RejectBall();
			}
			break;
		case STAGE2:
			// middle is moving now 
			if( balls < MAX_BALLS && topIR->Get() == BALL_NOT_VISIBLE )
			{
				// the middle sensor no longer senses a ball? time to stop and wait for another ball to show up
				if( middleIR->Get() == BALL_NOT_VISIBLE )
				{
					lifter->Set(COLLECTOR_STOP);
					collectorState = LOOKING_FOR_BALLS;
					balls++;
				}
			}
			else
			{
				// Already have enough balls, kick out any balls in the way and go back to looking which will eject any new balls that appear
				collectorState = LOOKING_FOR_BALLS;
				RejectBall();
			}

			break;
		case PREPARE_TO_SHOOT:
			// move backwards until top sensor does not sense a ball
			if( topIR->Get() == BALL_NOT_VISIBLE )
			{
				collectorState = OFF;
				lifter->Set( COLLECTOR_STOP );
				break;
			}
			lifter->Set( COLLECTOR_RUNSLOW_REVERSE );
			break;

		case SHOOTING:
			lifter->Set( COLLECTOR_RUNSLOW );
			Wait( 1 );
			lifter->Set( COLLECTOR_RUNFAST );
			Wait( .5 );
			balls = 0;
			collectorState = LOOKING_FOR_BALLS;
			break;
		}
	}	
}
