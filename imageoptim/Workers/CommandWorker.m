//
//  Worker.m
//
//  Created by porneL on 23.wrz.07.
//

#import "CommandWorker.h"
#include <unistd.h>
#import "../File.h"

@implementation CommandWorker

-(id)initWithFile:(File *)aFile
{
	if (self = [self init])
	{
		self.file = aFile;
	}
	return self;
}

-(BOOL)isRelatedTo:(File *)f
{
	return (f == file);
}

-(BOOL)parseLine:(NSString *)line
{
	/* stub */
	return NO;
}


-(void)parseLinesFromHandle:(NSFileHandle *)commandHandle
{
	NSData *temp;
	char inputBuffer[4096];
	NSInteger inputBufferPos=0;
	while((temp = [commandHandle availableData]) && [temp length])
	{
		const char *tempBytes = [temp bytes];
		NSInteger bytesPos=0, bytesLength = [temp length];

		while(bytesPos < bytesLength)
		{
			if (tempBytes[bytesPos] == '\n' || tempBytes[bytesPos] == '\r' || inputBufferPos == sizeof(inputBuffer)-1)
			{
				inputBuffer[inputBufferPos] = '\0';
				if ([self parseLine:[NSString stringWithUTF8String:inputBuffer]])
				{
					return;
				}
				inputBufferPos=0;bytesPos++;
			}
			else
			{
				inputBuffer[inputBufferPos++] = tempBytes[bytesPos++];
			}
		}
	}
}

-(NSTask *)taskWithPath:(NSString*)path arguments:(NSArray *)arguments;
{
	NSTask *task;
	task = [[NSTask alloc] init];

	NSLog(@"Launching %@ with %@",path,arguments);

	[task setLaunchPath: path];
	[task setArguments: arguments];

	// clone the current environment
	NSMutableDictionary*
	environment =[NSMutableDictionary dictionaryWithDictionary: [[NSProcessInfo processInfo] environment]];

    // set up for unbuffered I/O
	[environment setObject:@"YES" forKey:@"NSUnbufferedIO"];

    [task setEnvironment:environment];

	return task;
}

-(void)launchTask:(NSTask *)task
{
	@try
	{
		[task launch];

		if ([[NSUserDefaults standardUserDefaults] boolForKey:@"RunLowPriority"])
		{
			int pid = [task processIdentifier];
			if (pid > 1) setpriority(PRIO_PROCESS, pid, PRIO_MAX/2); // PRIO_MAX is minimum priority. POSIX is intuitive.
		}
	}
	@catch(NSException *e)
	{
		NSLog(@"Failed to launch %@ - %@",[self className],e);
	}
}

-(long)readNumberAfter:(NSString *)str inLine:(NSString *)line
{
	NSRange substr = [line rangeOfString:str];

	if (substr.length && [line length] > substr.location + [str length])
	{
		NSScanner *scan = [NSScanner scannerWithString:line];
		[scan setScanLocation:substr.location + [str length]];

		int res;
		if ([scan scanInt:&res])
		{
			return res;
		}
	}
	return 0;
}


-(NSTask *)taskForKey:(NSString *)key bundleName:(NSString *)resourceName arguments:(NSArray *)args
{
	NSString *executable = [self executablePathForKey:key bundleName:resourceName];

	if (!executable)
    {
        NSLog(@"Could not launch %@",resourceName);
        [file setStatus:@"err" order:8 text:[NSString stringWithFormat:NSLocalizedString(@"%@ failed to start",@"tooltip"),key]];
        return nil;
    }

	return [self taskWithPath:executable arguments:args];
}

-(NSString *)executablePathForKey:(NSString *)prefsName bundleName:(NSString *)resourceName
{
	NSUserDefaults *defs = [NSUserDefaults standardUserDefaults];
	NSString *path = nil;
    NSString *const kBundle = [prefsName stringByAppendingString:@"Bundle"];

	if ([defs boolForKey:kBundle])
	{
		if ((path = [[NSBundle mainBundle] pathForAuxiliaryExecutable:resourceName])
             && [[NSFileManager defaultManager] isExecutableFileAtPath:path]) {
			return path;
		} else {
			NSLog(@"There's no bundled executable for %@ at %@ - disabling",prefsName, path);
			[defs setBool:NO forKey:kBundle];
		}
	}

    NSString *const kPath = [prefsName stringByAppendingString:@"Path"];
	path = [defs stringForKey:kPath];
	if ([path length] && [[NSFileManager defaultManager] isExecutableFileAtPath:path])
	{
		return path;
	}

	NSLog(@"Can't find working executable for %@ - disabling",prefsName);
    NSBeep();
	[defs setBool:NO forKey:[prefsName stringByAppendingString:@"@Enabled"]];

	return nil;
}

-(NSString *)tempPath
{
    static int uid=0; if (uid==0) uid = getpid()<<12;
	return [NSTemporaryDirectory() stringByAppendingPathComponent: [NSString stringWithFormat:@"ImageOptim.%@.%x.%x.temp",[self className],[file hash]^[self hash],uid++]];
}

-(NSObject<WorkerQueueDelegate>*)delegate
{
	return file;
}
@synthesize file;
@end
