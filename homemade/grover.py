import numpy as np
from qiskit import QuantumCircuit, Aer, transpile, assemble, execute
from qiskit.visualization import plot_histogram

def grover_circuit(n, oracle):
    circuit = QuantumCircuit(n)
    
    # Step 1: Initialize superposition
    circuit.h(range(n))
    
    # Define Grover iteration
    def grover_iteration():
        # Apply Oracle
        circuit.append(oracle, range(n))
        
        # Apply Grover diffusion operator
        circuit.h(range(n))
        circuit.x(range(n))
        circuit.h(n-1)
        circuit.mct(list(range(n-1)), n-1)  # multi-controlled Toffoli
        circuit.h(n-1)
        circuit.x(range(n))
        circuit.h(range(n))

    # Step 3: Apply Grover iterations
    num_iterations = int(np.pi/4 * np.sqrt(2**n))
    for _ in range(num_iterations):
        grover_iteration()

    # Step 5: Measure
    circuit.measure_all()
    return circuit

# Example usage
n = 3  # Number of qubits
oracle = QuantumCircuit(n)
oracle.cz(0, n-1)  # Example oracle marking |101> state

grover_circ = grover_circuit(n, oracle)
simulator = Aer.get_backend('qasm_simulator')
compiled_circ = transpile(grover_circ, simulator)
qobj = assemble(compiled_circ)
result = execute(grover_circ, backend=simulator, shots=1024).result()
counts = result.get_counts()

print("Measurement results:", counts)
plot_histogram(counts)
