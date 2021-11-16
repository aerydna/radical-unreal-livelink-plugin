# RADiCAL LIVE <-> Unreal Live Link Plugin

This plugin contains retargeting assets for converting animation output from RADiCAL LIVE to an Unreal Engine editor session. It also contains the RADiCAL UE Live Link Client, a web socket client that receives data from a remote RADiCAL LIVE server and makes it available to Unreal Live Link.    

## Initial Setup Instructions

To install, download the latest release for your engine version from the [Releases section](https://github.com/get-rad/radical-unreal-livelink-plugin/releases). Unzip to your project `/Plugins` or engine `Marketplace` folder, then enable the plugin in your project.

## Enable the RADiCAL Simulation Data Stream

The simulation data stream is made available to allow you to test the integration between RADiCAL and Unreal Live Link.  The simulation data looks exactly like the data made available through a RADiCAL LIVE session. Once the simulated data stream is successfully implemented, you'll switch the RADiCAL UE Live Link Client to the room name with the actual real-time data.  

###  Start RADiCAL’s simulation data stream

1. From your web browser, visit RADiCAL’s webpage hosting the simulation stream: https://getrad.co/live-unreal-simulation. 

   _Note: you may have to log into the RADiCAL website to access this page._

2. Copy and save elsewhere the auto-generated room name (in this example: “simulation_0r2d2”). You will need it later.  

   _Note: do not change the room name_.

3. Click “Connect”.  After a brief countdown, you will see a simulated motion data scene.  

   _Note: you can click `Esc` to exit full-screen mode_.

###  Enable the RADiCAL UE Live Link Client
 
1. Launch Windows File Explorer and navigate to the `RADiCAL_UELiveLinkClient` folder. Open a terminal window from that location, by clicking on “`Open Window Powershell`” from the `File` menu. 

2. You are now in your “terminal” that shows you’re inside the `RADiCAL_UELiveLinkClient` folder.

3. Type the following command: ```.\RADiCAL_UELiveLinkClient.exe ––roomId <room_id>``` 

   Replace ```<room_id>``` with the room name auto-generated when you started the RADiCAL simulation stream. 

4. Click Enter. The RADiCAL UE Live Link Client will start. 
 
## Live Link Preview

After starting the Live Link stream through the RADiCAL UE Live Link Client, open the UE4 editor and go to Window -> Live Link. Go to Source -> Message Bus Source -> select the `RadLiveLink` source. You can now close this window.

We have provided two mannequins, one with the Epic skeleton and the other for the RADiCAL skeleton. The RADiCAL skeleton can be used as ground-truth for verifying animation data.

The Live Link data can be previewed inside the Skeleton, Skeletal Mesh, or Animation Blueprint windows for a given skeletal asset. 

On the right side, go to Preview Scene Settings, under Animation, change the Preview Controller to Live Link Preview Controller. Then, change the Live Link Subject Name to `RadicalPose`.

For Retarget Asset, select the corresponding Remap Asset file for that skeleton. For example, for the RADiCAL mannequin, choose the `RadToRadRemap` asset. For an Epic skeleton (either the provided Epic mannequin, or your project-specific character), choose the `BP_RadToEpicRemapAsset`. Assuming the target character uses an Epic skeleton, it should look as follows:

![Live Link Preview Setup](GALLERY_IMAGES/LiveLinkPreviewController.png)

## Live Link Source in AnimBP

To apply the incoming animation data to the characters in the scene, go to the AnimBP for the character and create a Live Link Pose node. Select the a) Live Link Subject Name and b) the Retarget Asset, like so:

![Live Link Pose node setup](GALLERY_IMAGES/LiveLinkPose.png)

Because of the difference between the RADiCAL skeleton (which defaults to a T pose) and the Unreal skeleton (which defaults to an A pose) we have to apply some rotations at the shoulder. Refer to the `AnimBP_EpicGames_Mannequin` AnimGraph and copy these transform nodes to your character:

![Transform Bone nodes](GALLERY_IMAGES/TransformBones.png)

## Rotation conversions for non-Epic skeletons (advanced users)

To account for some of the differences between the RADiCAL coordinate frame and Unreal, we have flipped the incoming LiveLink data's rotation and position axes. If you want to delve into the source C++, you can inspect the conversions at the `RadicalLiveLinkRemapAssetBase` class and its child classes. We expect that other skeletons will require different rotation adjustments, including swapping axes. We exposed three overridable methods to implement the root bone position, root bone rotation, and non-root bone rotation conversions. Please note that the AI output in RADiCAL uses the hip bone as a root, so position data should be mapped to the hips (or pelvis) in the target skeleton.
