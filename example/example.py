import mysim

divider =1_000_000

def submain():
    L = 10_000 # Number of sides of the microtubule
    T = 20_000 # Number of iterations
    N = 1500 # Number of particles
    
    # The following random numbers are based on a uniform distribution. For efficient (more efficient)
    # procedure we use a distribution from 0 till divider. As such the following numbers can be thought
    # how many times will a kinesin do x or y in one step.
    
    # For example one kinesin will step 0.1 times in an iteration. Or on average it will take 10 steps
    # so that a kinesin steps forward
    kon = 1e-3*divider  # Probability *  a kinesin binds during an iteration   
    koff = 1e-3*divider # Probability of a kinesin to unbind during an iteration
    kstep = 1e-1*divider # Probabiliy of a kinesin to step forward
    
    
    results = mysim.run(L, T, N, kstep, kon, koff)
    return results


if __name__ == "__main__":
    submain()