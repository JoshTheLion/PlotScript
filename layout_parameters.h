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

	// Plot data bounding rectangle params
	double xMax;				double yMax;
	double xMin;				double yMin;
	double xMid;				double yMid;

	// Flags to track whether the X or Y Axis has been drawn
	bool xAxis = false; bool yAxis = false;

	double txtScale;
};


#endif
