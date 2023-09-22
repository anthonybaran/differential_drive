/*
Everything in meters
Install GTK libraries on ubuntu: sudo apt-get install libgtk-3-dev
Compile code in terminal:  g++ -o test_differential_drive test_differential_drive.cpp `pkg-config --cflags --libs gtk+-3.0`
Then run the executable: ./test_differential_drive
*/

#include <iostream>
#include <gtk/gtk.h>

double robot_width = 0.5;
double joy_radius;
double joy_angle;

double output_turning_radius(double theta) 
{
    double outputMin = 0; // Minimum value of the output variable
    double outputMax = 8.5; // Maximum value of the output variable

    // Scale the input to the output range
    double scaleFactor = static_cast<double>(outputMax - outputMin) / 1.57;
    
    double output;
    if (theta > 0 && theta < 1.57) {
        output = static_cast<double>(scaleFactor * theta);
    }
    else if (theta > 1.57 && theta < 3.14) {
        output = static_cast<double>(scaleFactor * (theta - 2*(theta - 1.57)));
    }
    else if (theta > 3.14 && theta < 4.71) {
        output = static_cast<double>(scaleFactor * (theta - 3.14));
    }
    else if (theta > 4.71 && theta < 6.28) {
        output = static_cast<double>(scaleFactor * ((theta - 3.14) - 2*((theta - 3.14) - 1.57)));
    }
    else {
        output = 0;
    }

    return output;
}

double output_velocity(double joystick_radius)
{
    double outputMin = 0;
    double outputMax = 50;

    double scaleFactor = static_cast<double>(outputMax - outputMin) / 5.0; // divide by the max joystick radius
    double output = static_cast<double>(scaleFactor * joystick_radius);

    return output;
}

struct VelocityLabels {
    GtkWidget* right;
    GtkWidget* left;
};

void calculate_wheel_velocities(GtkRange* range, gpointer user_data) 
{
    double right_Wheel_velocity;
    double left_Wheel_velocity;

    if (joy_angle >= 0 && joy_angle <= 0.06) {
        right_Wheel_velocity = (output_velocity(joy_radius) / output_turning_radius(0.04)) * (output_turning_radius(0.04) - (robot_width / 2));
        left_Wheel_velocity = (output_velocity(joy_radius) / output_turning_radius(0.04)) * (output_turning_radius(0.04) + (robot_width / 2));
    }
    else if (1.57 > joy_angle && joy_angle > 0.06) {
        right_Wheel_velocity = (output_velocity(joy_radius) / output_turning_radius(joy_angle)) * (output_turning_radius(joy_angle) - (robot_width / 2));
        left_Wheel_velocity = (output_velocity(joy_radius) / output_turning_radius(joy_angle)) * (output_turning_radius(joy_angle) + (robot_width / 2));
    }
    else if (joy_angle == 1.57) {
        right_Wheel_velocity = output_velocity(joy_radius);
        left_Wheel_velocity = output_velocity(joy_radius);
    }
    else if (joy_angle > 1.57 && joy_angle < 3.08) {
        right_Wheel_velocity = (output_velocity(joy_radius) / output_turning_radius(joy_angle)) * (output_turning_radius(joy_angle) + (robot_width / 2));
        left_Wheel_velocity = (output_velocity(joy_radius) / output_turning_radius(joy_angle)) * (output_turning_radius(joy_angle) - (robot_width / 2));
    }
    else if (joy_angle >= 3.08 && joy_angle <= 3.20) {
        right_Wheel_velocity = (output_velocity(joy_radius) / output_turning_radius(3.10)) * (output_turning_radius(3.10) + (robot_width / 2));
        left_Wheel_velocity = (output_velocity(joy_radius) / output_turning_radius(3.10)) * (output_turning_radius(3.10) - (robot_width / 2));
    }
    else if (joy_angle > 3.20 && joy_angle < 4.71) {
        right_Wheel_velocity = -(output_velocity(joy_radius) / output_turning_radius(joy_angle)) * (output_turning_radius(joy_angle) + (robot_width / 2));
        left_Wheel_velocity = -(output_velocity(joy_radius) / output_turning_radius(joy_angle)) * (output_turning_radius(joy_angle) - (robot_width / 2));
    }
    else if (joy_angle == 4.71) {
        right_Wheel_velocity = -output_velocity(joy_radius);
        left_Wheel_velocity = -output_velocity(joy_radius);
    }
    else if (joy_angle > 4.71 && joy_angle < 6.22) {
        right_Wheel_velocity = -(output_velocity(joy_radius) / output_turning_radius(joy_angle)) * (output_turning_radius(joy_angle) - (robot_width / 2));
        left_Wheel_velocity = -(output_velocity(joy_radius) / output_turning_radius(joy_angle)) * (output_turning_radius(joy_angle) + (robot_width / 2));
    }
    else if (joy_angle >= 6.22) {
        right_Wheel_velocity = (output_velocity(joy_radius) / output_turning_radius(0.04)) * (output_turning_radius(0.04) - (robot_width / 2));
        left_Wheel_velocity = (output_velocity(joy_radius) / output_turning_radius(0.04)) * (output_turning_radius(0.04) + (robot_width / 2));
    }

    // Update label texts with the new wheel velocities
    VelocityLabels* labels = static_cast<VelocityLabels*>(user_data);

    gchar* right_label_text = g_strdup_printf("right wheel velocity: %.2f m/s", right_Wheel_velocity);
    gtk_label_set_text(GTK_LABEL(labels->right), right_label_text);
    g_free(right_label_text);

    gchar* left_label_text = g_strdup_printf("left wheel velocity: %.2f m/s", left_Wheel_velocity);
    gtk_label_set_text(GTK_LABEL(labels->left), left_label_text);
    g_free(left_label_text);
}


void on_slider1_changed(GtkRange* range, gpointer user_data) 
{
    double value = gtk_range_get_value(range);
    joy_radius = value;

    // Update label text with the new value
    gchar* label_text = g_strdup_printf("Joystick Radius: %.2f", value);
    GtkWidget* label = static_cast<GtkWidget*>(user_data);
    gtk_label_set_text(GTK_LABEL(label), label_text);
    g_free(label_text);
}

void on_slider2_changed(GtkRange* range, gpointer user_data) 
{
    double value = gtk_range_get_value(range);
    joy_angle = value;

    // Update label text with the new value
    gchar* label_text = g_strdup_printf("Joystick Angle: %.2f", value);
    GtkWidget* label = static_cast<GtkWidget*>(user_data);
    gtk_label_set_text(GTK_LABEL(label), label_text);
    g_free(label_text);   
}

int main(int argc, char* argv[]) 
{
    // Initialize GTK
    gtk_init(&argc, &argv);

    // Create the main window
    GtkWidget* window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    // Create two sliders
    GtkWidget* slider1 = gtk_scale_new_with_range(GTK_ORIENTATION_HORIZONTAL, 0, 5, 0.1);
    GtkWidget* slider2 = gtk_scale_new_with_range(GTK_ORIENTATION_HORIZONTAL, 0, 6.28, 0.01);
    gtk_range_set_value(GTK_RANGE(slider1), 0);
    gtk_range_set_value(GTK_RANGE(slider2), 0);

    // Create labels
    GtkWidget* slider_label1 = gtk_label_new("Joystick Radius: 0.00");
    GtkWidget* slider_label2 = gtk_label_new("Joystick Angle: 0.00");
    GtkWidget* right_wheel_label = gtk_label_new("right wheel velocity: 0.00 m/s");
    GtkWidget* left_wheel_label = gtk_label_new("left wheel velocity: 0.00 m/s");
    
    // Connect the slider signals to the respective callback functions
    g_signal_connect(slider1, "value-changed", G_CALLBACK(on_slider1_changed), slider_label1);
    g_signal_connect(slider2, "value-changed", G_CALLBACK(on_slider2_changed), slider_label2);
    VelocityLabels labels = {right_wheel_label, left_wheel_label};
    g_signal_connect(slider1, "value-changed", G_CALLBACK(calculate_wheel_velocities), &labels);
    g_signal_connect(slider2, "value-changed", G_CALLBACK(calculate_wheel_velocities), &labels);

    // Create a vertical box to hold the widgets
    GtkWidget* box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_container_add(GTK_CONTAINER(box), slider1);
    gtk_container_add(GTK_CONTAINER(box), slider2);
    gtk_container_add(GTK_CONTAINER(box), slider_label1);
    gtk_container_add(GTK_CONTAINER(box), slider_label2);
    gtk_container_add(GTK_CONTAINER(box), right_wheel_label);
    gtk_container_add(GTK_CONTAINER(box), left_wheel_label);

    // Add the box to the main window
    gtk_container_add(GTK_CONTAINER(window), box);

    // Show all widgets and start the GTK main loop
    gtk_widget_show_all(window);
    gtk_main();

    return 0;
}