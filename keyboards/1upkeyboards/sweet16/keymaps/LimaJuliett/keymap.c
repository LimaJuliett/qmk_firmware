#include QMK_KEYBOARD_H

#define TEXT 0
#define CODE 1
#define EMOJI 2
#define NUMPAD 3

// each custom keycode is a macro name (not every key needs a custom keycode)
enum custom_keycodes {
  LIMA_EMAIL = SAFE_RANGE,
  ANOTHER_EMAIL,
  A_THIRD_EMAIL,
  ADDRESS,
  COPY_TAB_PASTE,
  COPY_ALTTAB_PASTE,
  MULTICURSOR_SKIP
};

// tap dance keys
enum {
  TD_COPY,
  TD_PASTE,
  TD_MULTICURSOR_REMOVE
};

// CTRL-C on one tap, CTRL-SHIFT-C on two taps
void td_copy_function(qk_tap_dance_state_t *state, void *user_data) {
  if (state->count == 1) {
    tap_code16(LCTL(KC_C));
  }
  else if (state->count ==2) {
    tap_code16(LCTL(LSFT(KC_C)));
  }
}

// CTRL-V on one tap, CTRL-SHIFT-V on two taps
void td_paste_function(qk_tap_dance_state_t *state, void *user_data) {
  if (state->count == 1) {
    tap_code16(LCTL(KC_V));
  }
  else if (state->count ==2) {
    tap_code16(LCTL(LSFT(KC_V)));
  }
}

// Removes last multicursor on one tap, removes all multicursors on two taps
void td_multicursor_remove_function(qk_tap_dance_state_t *state, void *user_data) {
  if (state->count == 1) {
    tap_code16(LCTL(KC_U));
  }
  else if (state->count ==2) {
    tap_code(KC_ESC);
  }
}

qk_tap_dance_action_t tap_dance_actions[] = {
  [TD_COPY] = ACTION_TAP_DANCE_FN(td_copy_function),
  [TD_PASTE] = ACTION_TAP_DANCE_FN(td_paste_function),
  [TD_MULTICURSOR_REMOVE] = ACTION_TAP_DANCE_FN(td_multicursor_remove_function)
};

// names of the unicode characters (all emoji here)
enum unicode_names {
  HEART,
  SMILE,
  GRIN,
  UPSIDE_DOWN,
  SUNGLASSES,
  FROWN,
  SAD_TEAR,
  UNAMUSED,
  WINK_TONGUE
};

// mapping the unicode names to their respective unicode code points
const uint32_t PROGMEM unicode_map[] = {
  [HEART] = 0x2764,
  [SMILE] = 0x1F642,
  [GRIN] = 0x1F601,
  [UPSIDE_DOWN] = 0x1F643,
  [SUNGLASSES] = 0x1F60E,
  [FROWN] = 0x1F641,
  [SAD_TEAR] = 0x1F622,
  [UNAMUSED] = 0x1F612,
  [WINK_TONGUE] = 0x1F61C
};

// where keys are assigned functions
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [TEXT] = LAYOUT_ortho_4x4( // copying, pasting, commonly typed strings
    TD(TD_COPY),      TD(TD_PASTE),       LCTL(KC_X),     TO(TEXT),
    COPY_TAB_PASTE,   COPY_ALTTAB_PASTE,  XXXXXXX,        TO(CODE),
    ADDRESS,     ANOTHER_EMAIL,          A_THIRD_EMAIL,   TO(EMOJI),
    LIMA_EMAIL,       LCTL(KC_Z),         LCTL(KC_Y),     TO(NUMPAD)
  ),
  [CODE] = LAYOUT_ortho_4x4( // common keys for VSCode
    LALT(LCTL(KC_UP)),         LALT(KC_UP),         LALT(LSFT(KC_UP)),   TO(TEXT), // up/down: multicursor, shift line, copy line
    LALT(LCTL(KC_DOWN)),       LALT(KC_DOWN),       LALT(LSFT(KC_DOWN)), TO(CODE), // up/down: multicursor, shift line, copy line
    LCTL(KC_D),                MULTICURSOR_SKIP,    LCTL(LSFT(KC_L)),    TO(EMOJI), // multicursor to next, multicursor skip, multicursor all
    TD(TD_MULTICURSOR_REMOVE), LCTL(KC_Z),          LCTL(KC_Y),          TO(NUMPAD) // undo last multicursor placement, undo, redo
  ),
  [EMOJI] = LAYOUT_ortho_4x4( // emoji
    X(HEART),       X(GRIN),        X(SMILE),      TO(TEXT),
    X(SAD_TEAR),    X(FROWN),       X(UNAMUSED),   TO(CODE),
    X(WINK_TONGUE), X(UPSIDE_DOWN), X(SUNGLASSES), TO(EMOJI),
    UC_LINX,        UC_WIN,         UC_MAC,        TO(NUMPAD)  // switch to linux, windows (alt+numpad), and mac os unicode input modes
  ),
  [NUMPAD] = LAYOUT_ortho_4x4( // a normal keypad
    KC_7, KC_8,    KC_9,    TO(TEXT),
    KC_4, KC_5,    KC_6,    TO(CODE),
    KC_1, KC_2,    KC_3,    TO(EMOJI),
    KC_0, KC_PDOT, KC_BSPC, TO(NUMPAD)  // numpad dot and backspace
  )
};

// this function triggered on each keypress, macros are handled here
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  switch (keycode) {
    case LIMA_EMAIL:
      if (record->event.pressed) {
          SEND_STRING("LimaJuliett@comcast.net");
      }
      return false;
      break;
    case ANOTHER_EMAIL:
      if (record->event.pressed) {
          SEND_STRING("do.i.want.hack@gmail.com");
      }
      return false;
      break;
    case A_THIRD_EMAIL:
      if (record->event.pressed) {
          SEND_STRING("doctor.humperdink@gmail.com");
      }
      return false;
      break;
    case ADDRESS:
      if (record->event.pressed) {
          SEND_STRING("Name\tStreet Address\tCity\tState\tZip");
      }
      return false;
      break;
    case COPY_TAB_PASTE:
      if (record->event.pressed) {
          tap_code16(LCTL(KC_C));
          wait_ms(50);
          tap_code16(LCTL(KC_T));
          tap_code16(LCTL(KC_V));
          tap_code(KC_ENTER);
      }
      return false;
      break;
    case COPY_ALTTAB_PASTE:
      if (record->event.pressed) {
          tap_code16(LCTL(KC_C));
          wait_ms(50);
          register_code(KC_LALT);
          tap_code(KC_TAB);
          unregister_code(KC_LALT);
          wait_ms(1000);
          tap_code16(LCTL(KC_V));
      }
      return false;
      break;
    case MULTICURSOR_SKIP:
      if (record->event.pressed) {
          tap_code16(LCTL(KC_K));
          tap_code16(LCTL(KC_D));
      }
      return false;
      break;
  }
  return true;
}

// only for keyboards with rotary encoders (not present on mine)
#ifdef ENCODER_ENABLE
#include "encoder.h"
bool encoder_update_user(uint8_t index, bool clockwise) {
  if (index == 0) { /* First encoder */
    if (clockwise) {
      tap_code(KC_VOLU);
    } else {
      tap_code(KC_VOLD);
    }
  }
  return true;
}
#endif
