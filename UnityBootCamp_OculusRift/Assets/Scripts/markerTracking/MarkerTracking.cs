using UnityEngine;

using System;
using System.Threading;
using System.Collections;

public class MarkerTracking
{
	private TrackingDataProvider trackingDataProvider;
	private TrackingCalibration trackingCalibration;
	private Thread trackingDataThread;
	
	private byte playerMarkerID = 1;
	
	private Vector3 playerMarkerPosition;
	
	
	public MarkerTracking()
	{
		trackingDataProvider = new TrackingDataProvider();
		trackingCalibration = new TrackingCalibration();
		trackingDataThread = new Thread(new ThreadStart(trackingDataProvider.run));
		
		playerMarkerPosition = new Vector3(0f, 0.5f, 0f);
	}
	
	public void initialize()
	{
		trackingDataThread.Start();
	}
	
	public Vector3 playerPosition()
	{
		return trackingCalibration.mapToUnity(playerMarkerPosition);
	}
	
	public void update()
	{
		byte[] data = trackingDataProvider.latestData();
		// byte[] data = new byte[34];
		byte id = data[9];
		
		//
		// make sure calibration is finished
		// otherwise, look for calibration markers only
		//
		if (!trackingCalibration.calibrated)
		{
			if (trackingCalibration.referenceMarkerID <= id && id < trackingCalibration.referenceMarkerID + 4)
				trackingCalibration.setCalibrationMarkerCoordinate(	id,
																	BitConverter.ToDouble(data, 10),
																	BitConverter.ToDouble(data, 18));
			return;
		}
		
		//
		// update player position
		//
		if (id == playerMarkerID)
		{
			playerMarkerPosition.x = (float)BitConverter.ToDouble(data, 10);
			playerMarkerPosition.y = (float)BitConverter.ToDouble(data, 18);
		}
	}
}
			
//
//		Keep this list to remind me how to get which property
//

//		char tag = BitConverter.ToChar(data, 0);
//		Debug.Log("tag: " + (byte)tag);
//		if (tag != 32) Debug.Log("Error: Wrong tag (" + tag + ")");
//		
//		double timestamp = BitConverter.ToDouble(data, 1);
//		Debug.Log("t: " + timestamp); 
//		
//		char id = BitConverter.ToChar(data, 9);
//		Debug.Log("id: " + (byte)id);
		
//		double x = BitConverter.ToDouble(data, 10);
//		Debug.Log("x: " + x);
//		
//		double y = BitConverter.ToDouble(data, 18);
//		Debug.Log("y: " + y);
//		
//		double angle = BitConverter.ToDouble(data, 26);
//		Debug.Log("angle: " + angle);