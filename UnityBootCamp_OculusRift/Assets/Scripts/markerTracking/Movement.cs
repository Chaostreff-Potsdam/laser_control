using UnityEngine;

using System;
using System.Collections;

public class Movement : MonoBehaviour
{
	private MarkerTracking markerTracking;
	
	//Hack: smooth by interplation
	private float lastReceivedTime;
	private float animationTime = 0.5f;

	public Movement()
	{
		markerTracking = new MarkerTracking();
	}
	
	// Use this for initialization
	void Start()
	{
		markerTracking.initialize();
		transform.localPosition = new Vector3(-15.20321f, 2.028201f, -185.2529f);
	}
	
	// Update is called once per frame
	void Update ()
	{
		markerTracking.update();
		//Debug.Log(markerTracking.playerPosition());

		//Hack: smooth by interpolation
		Vector3 dest = markerTracking.playerPosition()+ new Vector3(-15.20321f, 2.028201f, -185.2529f);;
		transform.localPosition=dest;
//		dest.y = transform.localPosition.y;
//		Debug.Log(dest);
//
//		if(dest!= transform.localPosition){
//			animationTime = 0.5f;
//		}
//		if(animationTime>0){
//			float frac = Time.deltaTime/0.5f;
//			transform.localPosition = Vector3.Lerp(transform.position, dest, frac);
//			animationTime -= Time.deltaTime;
//		}
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
