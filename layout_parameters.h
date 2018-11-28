#ifndef LAYOUT_PARAMETERS_H
#define LAYOUT_PARAMETERS_H

/* This structure is to be used by the discrete-plot(DATA, OPTIONS) method
 * to encapsulate the plotting parameters of each evaluated expression, and
 * to communicate the data between helper functions. */
struct LayoutParams {
  
	double N = 20;		// Scale for the N x N bounding rect
	double A = 3;			// Vertical offset distance for title and axes labels
	double B = 3;			// Horizontal offset distance for title and axes labels
	double C = 2;			// Vertical offset distance for tick labels
	double D = 2;			// Horizontal offset distance for tick labels
	double P = 0.5;		// Size of points

	double txtScale;
	double xMax, yMax;	// Ordinate limits
	double xMin, yMin;	// Abscissa limits
	double xMid, yMid;	// Box center for text labels
	//double x0, y0;		// Maybe refactor in origin point location later

	// Flags to track whether the X or Y Axis has been drawn
	bool xAxis = false; bool yAxis = false;
	
};


#endif
