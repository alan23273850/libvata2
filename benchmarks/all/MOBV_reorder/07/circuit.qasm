OPENQASM 2.0;
include "qelib1.inc";
qreg qubits[15];

h qubits[14];
h qubits[13];
h qubits[11];
h qubits[9];
h qubits[7];
h qubits[5];
h qubits[3];
h qubits[1];
z qubits[14];
ccx qubits[0], qubits[1], qubits[14];
ccx qubits[2], qubits[3], qubits[14];
ccx qubits[4], qubits[5], qubits[14];
ccx qubits[6], qubits[7], qubits[14];
ccx qubits[8], qubits[9], qubits[14];
ccx qubits[10], qubits[11], qubits[14];
ccx qubits[12], qubits[13], qubits[14];
h qubits[14];
h qubits[13];
h qubits[11];
h qubits[9];
h qubits[7];
h qubits[5];
h qubits[3];
h qubits[1];
