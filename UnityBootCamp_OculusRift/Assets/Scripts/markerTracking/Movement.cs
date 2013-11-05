using UnityEngine;

using System;
using System.Collections;

public class Movement : MonoBehaviour
{
	private MarkerTracking markerTracking;
	
	
	public Movement()
	{
		markerTracking = new MarkerTracking();
	}
	
	// Use this for initialization
	void Start()
	{
		markerTracking.initialize();
	}
	
	// Update is called once per frame
	void Update ()
	{
		markerTracking.update();
		Debug.Log(markerTracking.playerPosition());
	}
	
//	// this will return the new player position in the end
//	void parseData(byte[] data)
//	{
//		debugPrintData(data);
//	}
//	
//	void debugPrintData(byte[] data)
//	{
		// TODO check if we get valid data
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
//	}
		
}
