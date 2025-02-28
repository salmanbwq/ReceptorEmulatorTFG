//
// Created by salman on 7/02/25.
//

#ifndef READRFID_H
#define READRFID_H

/**
 * 
 * @param scanner 
 * @param picc 
 * @return 
 */
uint8_t* readRFID(rc522_handle_t scanner, rc522_picc_t *picc);
#endif //READRFID_H
