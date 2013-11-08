using UnityEngine;

using System;
using System.Collections;

public class Movement : MonoBehaviour
{
	private MarkerTracking m_markerTracking;
	
	private Vector3 m_startingPosition;
	
	//Hack: smooth by interplation
	private float lastReceivedTime;
	private float animationTime = 0.5f;

	public Movement()
	{
		m_markerTracking = new MarkerTracking();
	}
	
	// Use this for initialization
	void Start()
	{
		m_startingPosition = transform.localPosition;
		m_markerTracking.initialize();
		// transform.localPosition = new Vector3(-15.20321f, 2.028201f, -250.2529f);
		// new Vector3(-8.902905f, -3.270549f, 209.7809f);// new Vector3(-15.20321f, 2.028201f, -185.2529f);
	}
	
	// Update is called once per frame
	void Update ()
	{
		m_markerTracking.update();
		//Debug.Log(markerTracking.playerPosition());

		//Hack: smooth by interpolation
		Vector3 dest = 5*m_markerTracking.playerPosition() + m_startingPosition;
		transform.localPosition=dest;
		
		
//		 animatedMovementTo(m_markerTracking.playerPosition());
	}
	
	void animatedMovementTo(Vector3 dest)
	{
		dest.y = transform.localPosition.y;
		Debug.Log(dest);

		if(dest!= transform.localPosition){
			animationTime = 0.5f;
		}
		
		if(animationTime>0){
			float frac = Time.deltaTime/0.5f;
			transform.localPosition = Vector3.Lerp(transform.position, dest, frac);
			animationTime -= Time.deltaTime;
		}
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
