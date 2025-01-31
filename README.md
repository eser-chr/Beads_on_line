# Beads_on_line
This is the main library I wrote as part of my masters thesis.

https://studenttheses.uu.nl/bitstream/handle/20.500.12932/39774/Thesis_Chris.pdf?sequence=1&isAllowed=y

# Use/Installation
One can install this small library, by downloading the project and then follow

cd <Path to folder>
mkdir build
cd build
cmake ..
make install


# Example of use
Please find an example file in the example folder

# Main Idea

The main idea of this project is to simulate a bulk of kinesins walking on a microtubule.
In a more abstract or mathematical way one can imagine a 1D discrete axon. This axon represents the microtubule and in this
simulation is discrete. The number of sides of this axon is noted as L.

The kinesins can be thought as particles or beads that might reside on a side or hover above it. Sadly the axon is aware of that kinesin only if it is bound to it. Following, that idea we can define a state for a kinesin, either bound or unbound plus has a position i.e the side of the microtubule. 

In this simulation we are intrested to know the position of all the kinesins no matter their state.


## Dynamics
The evolution of the system follows the following rules:
1) At each iteration we randomly define a permutation of all the kinesins and we subiterate over them.
2) During the sub-iteration we pick a kinesin and we try to bind/unbind that kinesin if initially it was unbound/bound accordingly. 
Then if the final state of that kinesin is bound we try to move forward.

By the word try we mean generate a random number and check if it is smaller than the given probabilities.



# Output
For historical reasons the output is a vector of 10 snapshots of the system. These 10 snapshots are equidistant in time.
Each snapshot simply contains the position of all kinesins.
