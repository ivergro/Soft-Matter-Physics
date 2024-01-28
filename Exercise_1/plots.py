import matplotlib.pyplot as plt
import os
import numpy as np
import re

#-----------Plotters------------
def draw_rectangle_hits():
    return 0

def draw_histograms(filepath, filename, name, save=False):
    """
    Note
    ----
    This func does not draw correctly amount of samples, because the samples are very spread.
    But with enough samples, the histogram would distribute the samples towards the actuall distribution.
    """
    data = load_vector(filepath + filename)
    # fig, ax = plt.subplots()
    # ax.hist(vec, density=True, bins=min(30, len(vec)//2))
    # ax.set_ylim(0,1)
    # Calculate relative frequencies
    # n, bins, _ = plt.hist(vec, bins=len(vec)//100)
    # relative_freq = n / len(vec)
    # # print(n, relative_freq)
    # # Create a histogram with relative frequencies
    # plt.bar(bins[:-1], relative_freq, width=(bins[1] - bins[0]))

    # plt.xlabel("Value")
    # plt.ylabel("Relative Frequency")
    # plt.show()
        # Calculate the histogram
    # m, bins, _ = plt.hist(data, density=True, bins=len(data)//100, label="Numerical")

    # Calculate relative frequencies
    # relative_freq = m / len(data)

    # Analytical expression
    stop = 2
    X = np.linspace(0, stop, 100000)
    n = int(name[-1])
    c = (n+1)/(stop**(n+1))
    Y = analytical(c,X,n)#*100*stop

    # Create a histogram with relative frequencies
    # plt.bar(bins[:-1], relative_freq, width=(bins[1] - bins[0]), label="Numerical")
    plt.hist(data, density=True, bins=min(30, len(data)//2), label="Numerical")
    plt.plot(X,Y, label="Analytical")

    plt.legend()
    plt.title(name)
    plt.xlabel("Value")
    plt.ylabel("Relative Frequency")

    # Create the directory if it doesn't exist
    if save:
        new_filepath = "./figures/ex_2_2/" + filename[:-4]
        directory_path = os.path.dirname(new_filepath)
        if not os.path.exists(directory_path):
            os.makedirs(directory_path)
        plt.savefig(new_filepath)
        if not os.path.exists(filepath + filename):
            print(f"Could not create the file '{filepath + filename}'")
        else:
            print("Figure saved in local path: " + new_filepath)
    else:
        plt.show()

def analytical(c,x,n):
    return c*x**n

#Ex 2.3
def draw_unit_circle(filepath, filename, save=False):
    data = load_pair_vector(filepath + filename)

    fig, axes = plt.subplots(1,2)

    circle_coord = axes[0]
    xy_coords = axes[1]

    #Circle coordinates
    r_val, theta_val = zip(*data)
    circle_coord.plot(r_val, theta_val, ".")
    circle_coord.set_ylabel("Theta")
    circle_coord.set_xlabel("r")
    circle_coord.set_title("Polar coordinates")

    #XY coordinates
    x_val = np.zeros(len(data))
    y_val = np.zeros(len(data))
    i = 0
    for r,theta in data:
        x_val[i], y_val[i] = r*np.sin(theta), r*np.cos(theta)
        i +=1
    xy_coords.plot(x_val, y_val, ".")
    xy_coords.set_ylabel("y")
    xy_coords.set_xlabel("x")
    xy_coords.set_title("XY Coordinates")

    if save:
        new_filepath = "./figures/ex_2_3/" + filename[:-4]
        directory_path = os.path.dirname(new_filepath)
        if not os.path.exists(directory_path):
            os.makedirs(directory_path)
        else:
            print("Figure saved in local path: " + new_filepath)
        fig.savefig(new_filepath)
    else:
        plt.show()
    
def box_muller_plots(filepath, filename, save=False):
    data = load_pair_vector(filepath + filename)
    x, y = zip(*data)

    X = np.linspace(-10,10,10000)
    gaussian = lambda x,mu,sigma: 1/(sigma*np.sqrt(2*np.pi))*np.exp(-((x - mu)/sigma)**2/2)
    Y = gaussian(X,2,np.sqrt(5))*5000

    if save:
        new_filepath = "./figures/ex_2_3/"
        directory_path = os.path.dirname(new_filepath)
        if not os.path.exists(directory_path):
            os.makedirs(directory_path)
        else:
            print("Figures saved in local path: " + new_filepath)
        hb = plt.hexbin(x, y, gridsize=15, cmap='plasma')
        plt.colorbar(hb, label='Counts')
        plt.title("2D Gaussian with N(2,5) with " + str(len(x)) + " samples")
        plt.savefig(new_filepath + "shifted_hexbin_N="+str(len(x)) + ".png")

        plt.clf()
        plt.hist(x, bins=32)
        plt.plot(X, Y, label="Analytical")
        plt.title("Gaussian for X with N(2,5) with " + str(len(x)) + " samples")
        plt.ylabel("Counts")
        plt.xlim([-6,10])
        plt.legend()
        plt.savefig(new_filepath + "shifted_X_N="+str(len(x)) + ".png")

        plt.clf()
        plt.hist(y, bins=32)
        plt.plot(X, Y, label="Analytical")
        plt.title("Gaussian for Y with N(2,5) with " + str(len(x)) + " samples")
        plt.ylabel("Counts")
        plt.xlim([-6,10])
        plt.legend()
        plt.savefig(new_filepath + "shifted_Y_N="+str(len(x)) + ".png")
    else:
        #2D hexbin with scaled colorbar
        hb = plt.hexbin(x, y, gridsize=15, cmap='plasma')
        plt.colorbar(hb, label='Counts')
        plt.title("2D Gaussian with N(0,1) with " + str(len(x)) + " samples")
        plt.show()

        plt.hist(x, bins=16)
        plt.plot(X, Y, label="Analytical")
        plt.title("Gaussian for X with N(0,1) with " + str(len(x)) + " samples")
        plt.ylabel("Counts")
        plt.legend()
        plt.xlim([-4,4])
        plt.show()

        plt.hist(y, bins=16)
        plt.plot(X, Y, label="Analytical")
        plt.title("Gaussian for Y with N(0,1) with " + str(len(x)) + " samples")
        plt.ylabel("Counts")
        plt.legend()
        plt.xlim([-4,4])
        plt.show()

def plot_E_of_t(filepath, filename_E, filename_t, save=False):
    # Create a plot of energy vs. timesteps
    timesteps = load_vector(filepath + filename_t)
    energies =  load_vector(filepath + filename_E)
    plt.plot(timesteps, energies, marker='o', linestyle='-', color='b', label='Energy')

    # Add labels and a legend
    plt.xlabel('Timesteps')
    plt.ylabel('Energy')
    plt.title('Energy vs. Timesteps')
    plt.legend()

    # Show the plot or save it to a file
    # plt.show()  # Use this to display the plot interactively
    plt.savefig('energy_vs_timesteps.png')  # Use this to save the plot to a file

def analytical_G(x, p):
    A = 1/(p+1/(2*p))
    if x <= p:
        return A
    else:
        return A/p*x*np.exp(p**2 - x**2)

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


def G_value_plot(filepath, filename, save=False):
    #G_value plot
    G_num = load_vector(filepath + filename)
    X = np.linspace(0,10, 10000)
    int_values = [int(match.group()) for match in re.finditer(r'\d+', filename)] #Saving L value in filename to take averages
    print(int_values)
    p = float(str(int_values[1]) + "." + str(int_values[2]))
    G_analytical = [analytical_G(x, p) for x in X]

    plt.clf()
    plt.hist(G_num, density=True,label="Numericly sampled points")
    plt.plot(X, G_analytical, label="Analytical g(x)")
    plt.legend()
    plt.title("Sampled points over g(x) with p=" + str(p))

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
# rejection_method(filepath, filename, False)
# G_value_plot(filepath, "G_vals" + filename[8:], False)

filepath = "./data/add_inv/"
filename1 = "1_mu=10.000000_N=10000.txt"
filename2 = "2_N=10000.txt"
draw_histograms(filepath, filename1, name="1", save=False)
