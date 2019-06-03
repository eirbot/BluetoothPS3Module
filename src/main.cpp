#include "Arduino.h"
#include <PS3BT.h>

USB Usb;

BTD Btd(&Usb);
PS3BT PS3(&Btd);

/* Liste des données envoyées :
 *
 * RX + valeur //Joystique droite axe X
 * RY + valeur //Joystique droite axe Y
 *
 * LX + valeur //Joystique gauche axe X
 * LY + valeur //Joystique gauche axe Y
 *
 * R2 + valeur //Gachette droite
 * L2 + valeur //Gachette gauche
 *
 * BV + valeur //Bouton triangle
 * B# + valeur //Bouton carrée
 * BX + valeur //Bouton croix
 * BO + valeur //Bouton rond
 *
 * BH + valeur //Bouton Haut
 * BB + valeur //Bouton Bas
 * BG + valeur //Bouton Gauche
 * BD + valeur //Bouton Droite
 *
 * BR + valeur //Bouton droite R1
 * BL + valeur //Bouton gauche L1
 *
 * JR + valeur //Bouton joystique droite
 * JL + valeur //Bouton joystique gauche
 *
 * SE + valeur //Bouton select
 * ST + valeur //Bouton start
 * PS + valeur //Bouton PS/P3
 *
 */


void setup() {
	Serial.begin(9600);

	if (Usb.Init() == -1) {
		//Serial.print(F("\r\nOSC did not start"));
		while (1); //halt
	}
	//Serial.print(F("\r\nPS3 Bluetooth Library Started"));

}


// ANALOG déclarations
uint8_t R2_new,
		L2_new,
		R_joystick_X_new,
		L_joystick_X_new,
		R_joystick_Y_new,
		L_joystick_Y_new;

uint8_t R2_old = 0,
		L2_old = 0,
		R_joystick_X_old = 127,
		L_joystick_X_old = 127,
		R_joystick_Y_old = 127,
		L_joystick_Y_old = 127;

// DIGITAL déclarations

bool boutonSquare_new,
		boutonTriangle_new,
		boutonCircle_new,
		boutonCross_new,
		boutonUp_new,
		boutonDown_new,
		boutonLeft_new,
		boutonRight_new,
		boutonStart_new,
		boutonSelect_new,
		boutonPS_new,
		boutonR1_new,
		boutonL1_new,
		boutonR3_new,
		boutonL3_new;

bool boutonSquare_old = false,
		boutonTriangle_old = false,
		boutonCircle_old = false,
		boutonCross_old = false,
		boutonUp_old = false,
		boutonDown_old = false,
		boutonLeft_old = false,
		boutonRight_old = false,
		boutonStart_old = false,
		boutonSelect_old = false,
		boutonPS_old = false,
		boutonR1_old = false,
		boutonL1_old = false,
		boutonR3_old = false,
		boutonL3_old = false;

// Programme principal

uint8_t key, mode_switch;
void loop() {

	Usb.Task();

	if (PS3.PS3Connected || PS3.PS3NavigationConnected) {


		// Changement de mode de type de manette
		if (Serial.available()) {
			key = (uint8_t) Serial.read();
			if (mode_switch == 0) {
				if (key == 'M') {
					//Code de demande changement de mode recu
					mode_switch = 1;
				}
			} else if (mode_switch == 1) {
				if (key == '2') {
					PS3.setLedOff();
					PS3.setLedOn(LED2);
					Serial.print("M");
					Serial.print("2");
					Serial.print("\n");
				} else if (key == '3') {
					PS3.setLedOff();
					PS3.setLedOn(LED3);
					Serial.print("M");
					Serial.print("3");
					Serial.print("\n");
				} else if (key == '4') {
					PS3.setLedOff();
					PS3.setLedOn(LED4);
					Serial.print("M");
					Serial.print("4");
					Serial.print("\n");
				} else {
					PS3.setLedOff();
					PS3.setLedOn(LED1);
					Serial.print("M");
					Serial.print("1");
					Serial.print("\n");
				}
				mode_switch = 0;
			}
		}

		// ANALOG joysticks

		R2_new = PS3.getAnalogButton(R2);
		L2_new = PS3.getAnalogButton(L2);
		R_joystick_X_new = PS3.getAnalogHat(RightHatX);
		L_joystick_X_new = PS3.getAnalogHat(LeftHatX);
		R_joystick_Y_new = PS3.getAnalogHat(RightHatY);
		L_joystick_Y_new = PS3.getAnalogHat(LeftHatY);


		//Joystick de droite
		if (R_joystick_X_new != R_joystick_X_old) {
			R_joystick_X_old = R_joystick_X_new;
			Serial.print("R");
			Serial.print("X");
			Serial.write(R_joystick_X_new);
			Serial.print("\n");
		}

		if (R_joystick_Y_new != R_joystick_Y_old) {
			R_joystick_Y_old = R_joystick_Y_new;
			Serial.print("R");
			Serial.print("Y");
			Serial.write(R_joystick_Y_new);
			Serial.print("\n");
		}

		//Joystick de gauche
		if (L_joystick_X_new != L_joystick_X_old) {
			L_joystick_X_old = L_joystick_X_new;
			Serial.print("L");
			Serial.print("X");
			Serial.write(L_joystick_X_new);
			Serial.print("\n");
		}

		if (L_joystick_Y_new != L_joystick_Y_old) {
			L_joystick_Y_old = L_joystick_Y_new;
			Serial.print("L");
			Serial.print("Y");
			Serial.write(L_joystick_Y_new);
			Serial.print("\n");
		}


		if (R2_new != R2_old) {
			R2_old = R2_new;
			Serial.print("R");
			Serial.print("2");
			Serial.write(R2_new);
			Serial.print("\n");
		}

		if (L2_new != L2_old) {
			L2_old = L2_new;
			Serial.print("L");
			Serial.print("2");
			Serial.write(L2_new);
			Serial.print("\n");
		}

		// DIGITAL boutons


		boutonSquare_new = PS3.getButtonPress(SQUARE);
		boutonTriangle_new = PS3.getButtonPress(TRIANGLE);
		boutonCircle_new = PS3.getButtonPress(CIRCLE);
		boutonCross_new = PS3.getButtonPress(CROSS);
		boutonUp_new = PS3.getButtonPress(UP);
		boutonDown_new = PS3.getButtonPress(DOWN);
		boutonLeft_new = PS3.getButtonPress(LEFT);
		boutonRight_new = PS3.getButtonPress(RIGHT);
		boutonStart_new = PS3.getButtonPress(START);
		boutonSelect_new = PS3.getButtonPress(SELECT);
		boutonPS_new = PS3.getButtonPress(PS);
		boutonR1_new = PS3.getButtonPress(R1);
		boutonL1_new = PS3.getButtonPress(L1);
		boutonR3_new = PS3.getButtonPress(R3);
		boutonL3_new = PS3.getButtonPress(L3);
		

		if (boutonSquare_new != boutonSquare_old) {
			boutonSquare_old = boutonSquare_new;
			Serial.print("B");
			Serial.print("#");
			Serial.print(((boutonSquare_new) ? "1" : "0"));
			Serial.print("\n");
		}

		if (boutonTriangle_new != boutonTriangle_old) {
			boutonTriangle_old = boutonTriangle_new;
			Serial.print("B");
			Serial.print("V");
			Serial.print(((boutonTriangle_new) ? "1" : "0"));
			Serial.print("\n");
		}

		if (boutonCircle_new != boutonCircle_old) {
			boutonCircle_old = boutonCircle_new;
			Serial.print("B");
			Serial.print("O");
			Serial.print(((boutonCircle_new) ? "1" : "0"));
			Serial.print("\n");
		}

		if (boutonCross_new != boutonCross_old) {
			boutonCross_old = boutonCross_new;
			Serial.print("B");
			Serial.print("X");
			Serial.print(((boutonCross_new) ? "1" : "0"));
			Serial.print("\n");
		}

		if (boutonUp_new != boutonUp_old) {
			boutonUp_old = boutonUp_new;
			Serial.print("B");
			Serial.print("H");
			Serial.print(((boutonUp_new) ? "1" : "0"));
			Serial.print("\n");
		}

		if (boutonDown_new != boutonDown_old) {
			boutonDown_old = boutonDown_new;
			Serial.print("B");
			Serial.print("B");
			Serial.print(((boutonDown_new) ? "1" : "0"));
			Serial.print("\n");
		}

		if (boutonLeft_new != boutonLeft_old) {
			boutonLeft_old = boutonLeft_new;
			Serial.print("B");
			Serial.print("G");
			Serial.print(((boutonLeft_new) ? "1" : "0"));
			Serial.print("\n");
		}

		if (boutonRight_new != boutonRight_old) {
			boutonRight_old = boutonRight_new;
			Serial.print("B");
			Serial.print("D");
			Serial.print(((boutonRight_new) ? "1" : "0"));
			Serial.print("\n");
		}

		if (boutonStart_new != boutonStart_old) {
			boutonStart_old = boutonStart_new;
			Serial.print("S");
			Serial.print("T");
			Serial.print(((boutonStart_new) ? "1" : "0"));
			Serial.print("\n");
		}

		if (boutonSelect_new != boutonSelect_old) {
			boutonSelect_old = boutonSelect_new;
			Serial.print("S");
			Serial.print("E");
			Serial.print(((boutonSelect_new) ? "1" : "0"));
			Serial.print("\n");
		}

		if (boutonPS_new != boutonPS_old) {
			boutonPS_old = boutonPS_new;
			Serial.print("P");
			Serial.print("S");
			Serial.print(((boutonPS_new) ? "1" : "0"));
			Serial.print("\n");
		}


		if (boutonR1_new != boutonR1_old) {
			boutonR1_old = boutonR1_new;
			Serial.print("B");
			Serial.print("R");
			Serial.print(((boutonR1_new) ? "1" : "0"));
			Serial.print("\n");
		}

		if (boutonL1_new != boutonL1_old) {
			boutonL1_old = boutonL1_new;
			Serial.print("B");
			Serial.print("L");
			Serial.print(((boutonL1_new) ? "1" : "0"));
			Serial.print("\n");
		}

		if (boutonR3_new != boutonR3_old) {
			boutonR3_old = boutonR3_new;
			Serial.print("J");
			Serial.print("R");
			Serial.print(((boutonR3_new) ? "1" : "0"));
			Serial.print("\n");
		}

		if (boutonL3_new != boutonL3_old) {
			boutonL3_old = boutonL3_new;
			Serial.print("J");
			Serial.print("L");
			Serial.print(((boutonL3_new) ? "1" : "0"));
			Serial.print("\n");
		}

//
//		if (PS3.getButtonClick(TRIANGLE)) {
//			Serial.print("Rumble Off\n");
//			PS3.setRumbleOff();
//		}
//		if (PS3.getButtonClick(CIRCLE)) {
//			Serial.print("Rumble On\n");
//			PS3.setRumbleOn(RumbleHigh);
//		}

	}
}


