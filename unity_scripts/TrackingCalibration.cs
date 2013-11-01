﻿using UnityEngine;

using System.Collections;
using System.Drawing;

using Emgu.CV;
//using Emgu.Util;

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
	
	private PointF[] corners = new PointF[4]; // [12,13,14,15]
	private bool[] cornersCalibrated = new bool[4]{false, false, false, false};
	private HomographyMatrix homography;
	
	public void setCalibrationMarkerCoordinate(byte markerID, double x, double y)
	{
		int index = markerID - referenceMarkerID;
		if (index < 0 || 3 < index) return;
		
		corners[index] = new PointF((float)x, (float)y);
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
		GameObject playground = GameObject.Find("Playground");	
		Bounds playgroundBounds = playground.renderer.bounds;
		PointF[] playgroundCorners = new PointF[]{
			new PointF(playgroundBounds.center.x + playgroundBounds.extents.x, playgroundBounds.center.z + playgroundBounds.extents.z),
			new PointF(playgroundBounds.center.x - playgroundBounds.extents.x, playgroundBounds.center.z + playgroundBounds.extents.z),
			new PointF(playgroundBounds.center.x - playgroundBounds.extents.x, playgroundBounds.center.z - playgroundBounds.extents.z),
			new PointF(playgroundBounds.center.x + playgroundBounds.extents.x, playgroundBounds.center.z - playgroundBounds.extents.z)};
		
		homography = computeHomography(corners, playgroundCorners);
		Debug.Log("Homography: " + homography.Data);
	}
	
	private HomographyMatrix computeHomography(PointF[] src, PointF[] dst)
	{
		HomographyMatrix H = CameraCalibration.FindHomography(src, dst, 0, 1);
		return H;
	}
}
