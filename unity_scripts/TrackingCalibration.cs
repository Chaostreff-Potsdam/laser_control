using UnityEngine;
using System.Collections;

struct Point2d 
{
	public double x;
	public double y;
	
	public Point2d(double _x, double _y)
	{
		x = _x; y = _y;
	}
}

public class TrackingCalibration
{
	public byte referenceMarkerID = 12;
	public bool calibrated = false;
	
	//
	//	Placement of calibration markers:
	//
	//	13----------12
	//	||			||
	//	||			||
	//	14----------15
	//
	
	private Point2d[] corners = new Point2d[4]; // [12,13,14,15]
	private bool[] cornersCalibrated = new bool[4]{false, false, false, false};

	public void setCalibrationMarkerCoordinate(byte markerID, double x, double y)
	{
		int index = markerID - referenceMarkerID;
		if (index < 0 || 3 < index) return;
		
		corners[index] = new Point2d(x, y);
		cornersCalibrated[index] = true;
		if (allCornersSet()) 
		{
			UnityEngine.Debug.Log("Calibration done!");
			computeCalibration();
		}
	}
	
	public Vector3 mapToUnity(Vector3 trackingCoordinates)
	{
		// TODO - this should do something!
		return trackingCoordinates;
	}
		
	private bool allCornersSet()
	{
		return 	cornersCalibrated[0] && cornersCalibrated[1] && 
				cornersCalibrated[2] && cornersCalibrated[3];
	}
	
	private void computeCalibration()
	{
		
	}
}
