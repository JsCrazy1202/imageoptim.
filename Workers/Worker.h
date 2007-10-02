//
//  Worker.h
//  ImageOptim
//
//  Created by porneL on 30.wrz.07.
//  Copyright 2007 __MyCompanyName__. All rights reserved.
//

#import <Cocoa/Cocoa.h>
@class WorkerQueue;

@interface Worker : NSObject {

}

-(BOOL)makesNonOptimizingModifications;

-(id)delegate;

-(void)run;

@end
