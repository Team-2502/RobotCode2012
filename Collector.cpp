#include <WPILib.h>
#include "Collector.h"

int Collector::balls = 0;
Victor *Collector::grabber = NULL;
Victor *Collector::lifter = NULL;
Sharp_IR *Collector::frontIR = NULL;
Sharp_IR *Collector::middleIR = NULL;
Sharp_IR *Collector::topIR = NULL;
CollectorState Collector::collectorState = OFF;

Collector::Collector(int bottomChannel, int middleChannel, int frontIRChannel, int middleIRChannel, int topIRChannel, int numberOfBalls )
{
	balls = numberOfBalls;
	grabber = new Victor(bottomChannel);
	lifter = new Victor(middleChannel);
	frontIR = new Sharp_IR(1, frontIRChannel);
	middleIR = new Sharp_IR(1, middleIRChannel);
	topIR = new Sharp_IR(1, topIRChannel);
	collectorState = OFF;
	collectorTask = new Task("2502Cl",(FUNCPTR)ThreadLoop);
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
void Collector::ManipulateRamp()
{
	collectorState = OFF;
	grabber->Set(STOP);
	lifter->Set(STOP);
}

// Turn off everything
void Collector::Stop()
{
	grabber->Set(STOP);
	lifter->Set(STOP);
	collectorState = OFF;
}

// Ejects ALL Balls - as long as it is running
void Collector::Eject()
{
	grabber->Set(-RUNFAST);
	lifter->Set(-RUNFAST);
	Wait( 1 );
	grabber->Set(STOP);
	lifter->Set(STOP);
	balls = 0;
	collectorState = LOOKING_FOR_BALLS;
}

// If collector is not doing something else, turn it on.
void Collector::RunCollector()
{
	if( collectorState == OFF )
	{
		collectorTask->Start();
		collectorState = LOOKING_FOR_BALLS;
	}
}

void Collector::RejectBall()
{
	grabber->Set( -RUNFAST );
	Wait( .5 );
	grabber->Set( STOP );
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
			if( frontIR->Get() == BLOCKED )
			{
				RejectBall();
			}
			Wait( .01 );
			break;
		case LOOKING_FOR_BALLS:
			// if we mount a sensor up front, we could turn on when we sense a ball if we have less than 2 balls
			if( frontIR->Get() == BLOCKED )
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
				grabber->Set(RUNFAST);
				if( middleIR->Get() == BLOCKED ) // Does bottom IR sense Ball?
				{
					// probably want to run this one slowly because we want to stop as soon as the IR sensor no longer senses the ball
					grabber->Set(RUNSLOW);
					lifter->Set(RUNSLOW);  
					Wait( .03 ); // enough time to transfer the ball beyond the pully
					grabber->Set(STOP);
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
			if( balls < MAX_BALLS && topIR->Get() == OPEN )
			{
				// the middle sensor no longer senses a ball? time to stop and wait for another ball to show up
				if( middleIR->Get() == OPEN )
				{
					lifter->Set(STOP);
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
			if( topIR->Get() == OPEN )
			{
				collectorState = OFF;
				lifter->Set( STOP );
				break;
			}
			lifter->Set( -RUNSLOW );
			break;

		case SHOOTING:
			lifter->Set( RUNSLOW );
			Wait( 1 );
			lifter->Set( RUNFAST );
			Wait( .5 );
			balls = 0;
			collectorState = LOOKING_FOR_BALLS;
			break;
		}
	}	
}
