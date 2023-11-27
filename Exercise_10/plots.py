import matplotlib.pyplot as plt
import os
import numpy as np
import re

#-----------Plotters------------

def plot_euler_integrator(filepath, filename, save = False):
    filename_first = "first_" + filename
    filename_second = "second" + filename


def rejection_method(filepath, filename, save=False):
    pdf = lambda x: 2*np.sqrt(1/np.pi)*np.exp(-x ** 2) #Normalized
    num_pdf = load_pair_vector(filepath + filename)
    # sampled_pdf_val = load_vector(filepath + filename)
    int_values = [int(match.group()) for match in re.finditer(r'\d+', filename)] #Saving L value in filename to take averages
    p = float(str(int_values[1]) + "." + str(int_values[2]))
    N = int_values[0]
    x = np.linspace(0, 5, N)
    y = pdf(x)
    num_x, num_y = zip(*num_pdf)

    # plt.plot(num_x, num_y, ".", label = "Numerical result")
    plt.hist(num_x, density=True, bins=min(30, len(num_pdf)//2), label="Numericly sampled points") #The y points are the correct distribution
    plt.plot(x, y, label = "Analytical PDF")
    plt.title(r"Rejection Method for $\rho$(x), c=1.5 p="+str(p))
    plt.legend()

    if save:
        new_filepath = "./figures/ex_2_3_1/" + filename[:-4]
        directory_path = os.path.dirname(new_filepath)
        if not os.path.exists(directory_path):
            os.makedirs(directory_path)
        else:
            print("Figure saved in local path: " + new_filepath)
        plt.savefig(new_filepath + ".png")
    else:
        plt.show()


# def G_value_plot(filepath, filename, save=False):
    # #G_value plot
    # G_num = load_vector(filepath + filename)
    # X = np.linspace(0,10, 10000)
    # int_values = [int(match.group()) for match in re.finditer(r'\d+', filename)] #Saving L value in filename to take averages
    # print(int_values)
    # p = float(str(int_values[1]) + "." + str(int_values[2]))
    # G_analytical = [analytical_G(x, p) for x in X]

    # plt.clf()
    # plt.hist(G_num, density=True,label="Numericly sampled points")
    # plt.plot(X, G_analytical, label="Analytical g(x)")
    # plt.legend()
    # plt.title("Sampled points over g(x) with p=" + str(p))

    # if save:
    #     new_filepath = "./figures/ex_2_3_1/" + filename[:-4]
    #     directory_path = os.path.dirname(new_filepath)
    #     if not os.path.exists(directory_path):
    #         os.makedirs(directory_path)
    #     else:
    #         print("Figure saved in local path: " + new_filepath)
    #     plt.savefig(new_filepath + ".png")
    # else:
    #     plt.show()




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

def load_pair_vector(filename):
    if not os.path.exists(filename):
        print(f"ERROR: File '{filename}' not found")
    else:
        with open(filename, "r") as file:
            # Read the entire file as a string
            file_contents = file.read()

        # Split and save values to list
        lines = file_contents.split("\n")
        temp_list = [tuple(line.split()) for line in lines] #Separating each line, then splitting them up in tuples
        list = [(float(x), float(y)) for x,y in temp_list]
        return list


    
#Lage switch case kjøremeny?
filepath = "./data/ex_2_3/"
# filename = "n=4_N=100000.txt"
# filename = "X_from_0_to_2_N=100000.txt"
# filename = "unit_disk_N=1000.txt"
# filename = "improved_unit_disk_N=1000.txt"
filename = "Box_Muller_shifted_N=10000.txt"
# filename = "Box_Muller_N=10000.txt"
# draw_histograms(filepath, filename, name="3/8*x^2", save=False)
# draw_unit_circle(filepath, filename, save=True)
# box_muller_plots(filepath, filename, True)




filepath = "./data_copy/10/"
filename_t = "temperatures_L=25_sweeps=100.txt"
filename_E = "energies_L=25_sweeps=100.txt"

# plot_E_of_t(filepath,filename_E, filename_t)

filepath = "data/ex_2_3_1/"
filename = "rej_meth_N=10000_p=0.600000.txt"
rejection_method(filepath, filename, True)
# G_value_plot(filepath, "G_vals" + filename[8:], True)

