/* 
 * File:   textdisplay.h
 * Author: steve
 *
 * Created on June 15, 2022, 10:31 PM
 */

#ifndef TEXTDISPLAY_H
#define	TEXTDISPLAY_H

#ifdef	__cplusplus
extern "C" {
#endif
    
#define MAX_MESSAGE_SIZE 200
    
void textdisplay_initialise();
void textdisplay_ctrl();
void textdisplay_roll_text();

#ifdef	__cplusplus
}
#endif

#endif	/* TEXTDISPLAY_H */

