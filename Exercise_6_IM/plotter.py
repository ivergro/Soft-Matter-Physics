import matplotlib.pyplot as plt
import os
import numpy as np
import re

def plot_avgE_avgM_of_t(filepath, filename_E, filename_M, filename_t, save=False):
    # Create a plot of energy vs. timesteps
    timesteps = np.array(load_vector(filepath + filename_t))
    energies =  np.array(load_vector(filepath + filename_E))
    magnetizations = np.array(load_vector(filepath + filename_M))
    int_values = [int(match.group()) for match in re.finditer(r'\d+', filename_E)] #Saving L value in filename to take averages
    N = int_values[0]**2
    avg_energies = energies/N
    avg_magnetization = magnetizations/N
    plt.plot(timesteps, avg_energies, linestyle='-', color='g', label=r'$\overline{E}$')
    plt.plot(timesteps, avg_magnetization, linestyle='-', color='r', label=r'$\overline{M}$')

    # Add labels and a legend
    plt.xlabel('Timesteps')
    # plt.ylabel('Energy')
    plt.title(r'$\overline{E}$ across time')
    plt.legend()

    # Show the plot or save it to a file
    # plt.show()  # Use this to display the plot interactively
    if save:
        new_filepath = "./figures/ex_2_2/" + filename_E[:-4]
        directory_path = os.path.dirname(new_filepath)
        if not os.path.exists(directory_path):
            os.makedirs(directory_path)
        plt.savefig(new_filepath)
        if not os.path.exists(filepath + filename_E):
            print(f"Could not create the file '{filepath + filename_E}'")
        else:
            print("Figure saved in local path: " + new_filepath)
    else:
        plt.show()

def plot_Cv(filepath, filename_Cv, filename_T, save=False):
    # Create a plot of specific_heat vs. temp
    Temps = np.array(load_vector(filepath + filename_T))
    specific_heat =  np.array(load_vector(filepath + filename_Cv))
    int_values = [match.group() for match in re.finditer(r'\d+', filepath)] #Saving L value in filename to take averages

    plt.plot(Temps, specific_heat, linestyle='-', color='g', label=r'C_V(T)')

    # Add labels and a legend
    plt.xlabel('Temperature')
    # plt.ylabel('Energy')
    plt.title('Specific heat for L='+int_values[0])
    plt.legend()

    # Show the plot or save it to a file
    # plt.show()  # Use this to display the plot interactively
    if save:
        new_filepath = "./figures/ex_6/" + filename_Cv[:-4]
        directory_path = os.path.dirname(new_filepath)
        if not os.path.exists(directory_path):
            os.makedirs(directory_path)
        plt.savefig(new_filepath)
        if not os.path.exists(filepath + filename_Cv):
            print(f"Could not create the file '{filepath + filename_Cv}'")
        else:
            print("Figure saved in local path: " + new_filepath)
    else:
        plt.show()

def plot_susceptibility(filepath, filename_X, filename_T, save=False):
    # Create a plot of specific_heat vs. temp
    Temps = np.array(load_vector(filepath + filename_T))
    specific_heat =  np.array(load_vector(filepath + filename_X))
    int_values = [match.group() for match in re.finditer(r'\d+', filepath)] #Saving L value in filename to take averages

    plt.plot(Temps, specific_heat, linestyle='-', color='g', label=r'X(T)')

    # Add labels and a legend
    plt.xlabel('Temperature')
    # plt.ylabel('Energy')
    plt.title('Susceptibility for L='+int_values[0])
    plt.legend()

    # Show the plot or save it to a file
    # plt.show()  # Use this to display the plot interactively
    if save:
        new_filepath = "./figures/ex_6/" + filename_X[:-4]
        directory_path = os.path.dirname(new_filepath)
        if not os.path.exists(directory_path):
            os.makedirs(directory_path)
        plt.savefig(new_filepath)
        if not os.path.exists(filepath + filename_X):
            print(f"Could not create the file '{filepath + filename_X}'")
        else:
            print("Figure saved in local path: " + new_filepath)
    else:
        plt.show()


#----------File loaders--------------
def load_vector(filename):
    if not os.path.exists(filename):
        print(f"ERROR: File '{filename}' not found")
    else:
        with open(filename, "r") as file:
            # Read the entire file as a string
            file_contents = file.read()

        # Split and save values to list
        lines = file_contents.split("\n")
        list = [float(item) for item in lines]
        return list

#-----------Help Functions------------
def calculate_susceptibiliy(energies):
    expected_squared_E = []


T = 0
L = 50
sweeps = 1000
filepath = "./data/"+str(T)+"/"
filename_t = "timesteps_L="+str(L)+"_sweeps="+str(sweeps)+".txt"
filename_E = "energies_L="+str(L)+"_sweeps="+str(sweeps)+".txt"
filename_M = "magnetizations_L="+str(L)+"_sweeps="+str(sweeps)+".txt"

filepath_2 = "./data/L="+str(L)+"/"
filename_Cv = "specific_heat_sweeps="+str(sweeps)+".txt"
filename_X = "susceptibility_sweeps="+str(sweeps)+".txt"
filename_T = "Temps"
# plot_avgE_avgM_of_t(filepath, filename_E, filename_M, filename_t, True)

plot_Cv(filepath_2, filename_Cv, filename_T)
plot_susceptibility(filepath_2,filename_X,filename_T)