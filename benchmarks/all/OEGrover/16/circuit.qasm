OPENQASM 2.0;
include "qelib1.inc";
qreg qubits[32];

x qubits[0];
x qubits[2];
x qubits[4];
x qubits[6];
x qubits[8];
x qubits[10];
x qubits[12];
x qubits[14];
ccx qubits[0], qubits[1], qubits[17];
ccx qubits[2], qubits[17], qubits[18];
ccx qubits[3], qubits[18], qubits[19];
ccx qubits[4], qubits[19], qubits[20];
ccx qubits[5], qubits[20], qubits[21];
ccx qubits[6], qubits[21], qubits[22];
ccx qubits[7], qubits[22], qubits[23];
ccx qubits[8], qubits[23], qubits[24];
ccx qubits[9], qubits[24], qubits[25];
ccx qubits[10], qubits[25], qubits[26];
ccx qubits[11], qubits[26], qubits[27];
ccx qubits[12], qubits[27], qubits[28];
ccx qubits[13], qubits[28], qubits[29];
ccx qubits[14], qubits[29], qubits[30];
cz qubits[30], qubits[15];
ccx qubits[14], qubits[29], qubits[30];
ccx qubits[13], qubits[28], qubits[29];
ccx qubits[12], qubits[27], qubits[28];
ccx qubits[11], qubits[26], qubits[27];
ccx qubits[10], qubits[25], qubits[26];
ccx qubits[9], qubits[24], qubits[25];
ccx qubits[8], qubits[23], qubits[24];
ccx qubits[7], qubits[22], qubits[23];
ccx qubits[6], qubits[21], qubits[22];
ccx qubits[5], qubits[20], qubits[21];
ccx qubits[4], qubits[19], qubits[20];
ccx qubits[3], qubits[18], qubits[19];
ccx qubits[2], qubits[17], qubits[18];
ccx qubits[0], qubits[1], qubits[17];
x qubits[0];
x qubits[2];
x qubits[4];
x qubits[6];
x qubits[8];
x qubits[10];
x qubits[12];
x qubits[14];
h qubits[0];
h qubits[1];
h qubits[2];
h qubits[3];
h qubits[4];
h qubits[5];
h qubits[6];
h qubits[7];
h qubits[8];
h qubits[9];
h qubits[10];
h qubits[11];
h qubits[12];
h qubits[13];
h qubits[14];
h qubits[15];
x qubits[0];
x qubits[1];
x qubits[2];
x qubits[3];
x qubits[4];
x qubits[5];
x qubits[6];
x qubits[7];
x qubits[8];
x qubits[9];
x qubits[10];
x qubits[11];
x qubits[12];
x qubits[13];
x qubits[14];
x qubits[15];
ccx qubits[0], qubits[1], qubits[17];
ccx qubits[2], qubits[17], qubits[18];
ccx qubits[3], qubits[18], qubits[19];
ccx qubits[4], qubits[19], qubits[20];
ccx qubits[5], qubits[20], qubits[21];
ccx qubits[6], qubits[21], qubits[22];
ccx qubits[7], qubits[22], qubits[23];
ccx qubits[8], qubits[23], qubits[24];
ccx qubits[9], qubits[24], qubits[25];
ccx qubits[10], qubits[25], qubits[26];
ccx qubits[11], qubits[26], qubits[27];
ccx qubits[12], qubits[27], qubits[28];
ccx qubits[13], qubits[28], qubits[29];
ccx qubits[14], qubits[29], qubits[30];
cz qubits[30], qubits[15];
ccx qubits[14], qubits[29], qubits[30];
ccx qubits[13], qubits[28], qubits[29];
ccx qubits[12], qubits[27], qubits[28];
ccx qubits[11], qubits[26], qubits[27];
ccx qubits[10], qubits[25], qubits[26];
ccx qubits[9], qubits[24], qubits[25];
ccx qubits[8], qubits[23], qubits[24];
ccx qubits[7], qubits[22], qubits[23];
ccx qubits[6], qubits[21], qubits[22];
ccx qubits[5], qubits[20], qubits[21];
ccx qubits[4], qubits[19], qubits[20];
ccx qubits[3], qubits[18], qubits[19];
ccx qubits[2], qubits[17], qubits[18];
ccx qubits[0], qubits[1], qubits[17];
x qubits[0];
x qubits[1];
x qubits[2];
x qubits[3];
x qubits[4];
x qubits[5];
x qubits[6];
x qubits[7];
x qubits[8];
x qubits[9];
x qubits[10];
x qubits[11];
x qubits[12];
x qubits[13];
x qubits[14];
x qubits[15];
h qubits[0];
h qubits[1];
h qubits[2];
h qubits[3];
h qubits[4];
h qubits[5];
h qubits[6];
h qubits[7];
h qubits[8];
h qubits[9];
h qubits[10];
h qubits[11];
h qubits[12];
h qubits[13];
h qubits[14];
h qubits[15];
z qubits[16];
