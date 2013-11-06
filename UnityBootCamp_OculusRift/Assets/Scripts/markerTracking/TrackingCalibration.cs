using UnityEngine;

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
	
	private PointF[] m_corners = new PointF[4]; // [12,13,14,15]
	private bool[] m_cornersCalibrated = new bool[4]{false, false, false, false};
	private HomographyMatrix m_homography;
	
	
	// HACK
	private Vector3 topLeftCorner;
	
	public void setCalibrationMarkerCoordinate(byte markerID, double x, double y)
	{
		int index = markerID - referenceMarkerID;
		UnityEngine.Debug.Log("Got calibration for corner " + index);
		if (index < 0 || 3 < index) return;
		
		m_corners[index] = new PointF((float)x, (float)y);
		m_cornersCalibrated[index] = true;
		if (allCornersSet()) 
		{
			UnityEngine.Debug.Log("Calibration done!");
			calibrated = true;
			// computeCalibration();
			
			// HACK
			topLeftCorner = new Vector3(m_corners[1].X, 0, m_corners[1].Y);
		}
	}
	
	public Vector3 mapToUnity(Vector3 trackingCoordinates)
	{
		// TODO - this should do something!
		// HACK
		return trackingCoordinates - topLeftCorner;
	}
		
	private bool allCornersSet()
	{
		return 	m_cornersCalibrated[0] && m_cornersCalibrated[1] && 
				m_cornersCalibrated[2] && m_cornersCalibrated[3];
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
		
		m_homography = computeHomography(m_corners, playgroundCorners);
		Debug.Log("Homography: " + m_homography.Data);
	}
	
	private HomographyMatrix computeHomography(PointF[] src, PointF[] dst)
	{
		HomographyMatrix H = CameraCalibration.FindHomography(src, dst, 0, 1);
		return H;
	}
}
