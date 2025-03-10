#pragma once

/* Universal Chess Interface is a protocol that chess GUIs use to talk to    */
/* chess engines. this function is called from `main` and handles            */
/* communication with the GUI. it's all just boring text parsing stuff, so   */
/* i'll spare you the details. do note that we only implement the bare       */
/* minimum required to play a game of chess, other stuff like pondering and  */
/* search options are not implemented.                                       */
/*                                                                           */
/* https://www.chessprogramming.org/UCI                                      */
void uci_run(const char *name, const char *author);
