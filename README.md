# RADiCAL LIVE <-> Unreal Live Link Plugin

This plugin contains retargeting assets for converting animation output from RADiCAL LIVE to an Unreal Engine editor session. It also contains the RADiCAL UE Live Link Client, a web socket client that receives data from a remote RADiCAL LIVE server and makes it available to Unreal Live Link.    

## Initial Setup Instructions

To install, download the latest release for your engine version from the [Releases section](https://github.com/get-rad/radical-unreal-livelink-plugin/releases). Unzip to your project `/Plugins` or engine `Marketplace` folder, then enable the plugin in your project.

## Enable the RADiCAL Simulation Data Stream

The simulation data stream is made available to allow you to test the integration between RADiCAL and Unreal Live Link.  The simulation data looks exactly like the data made available through a RADiCAL LIVE session. Once the simulated data stream is successfully implemented, you'll switch the RADiCAL UE Live Link Client to the room name with the actual real-time data.  

###  Start RADiCAL’s simulation data stream

1. From your web browser, visit RADiCAL’s webpage, live rooms section: https://getrad.co/live-rooms/

   _Note: you may have to log into the RADiCAL website to access this page._

2. Click to expand the Simulation dropdown. Select either the `Single Player` or `Multiplayer` room, then click `Open Room`, and you will be taken to the `Room details` page. In the lower right corner you will see the `3D Integrations` panel. We will need the `WEBSOCKET URL` and the `ROOM ID`. You can now click the `Enter room` button in the `Audience` panel. 

   _Note: the `Room details` page will remain open in another tab, you can get beck to it in the next step_.

3. Click “Connect”.  After the 3D assets are loaded, you will see a simulated motion data scene.  

   _Note: you can click `Esc` to exit full-screen mode_.

###  Enable the RADiCAL UE Live Link Client
 
1. Launch Windows File Explorer and navigate to the `RADiCAL_UELiveLinkClient` folder. Open a terminal window from that location, by clicking on “`Open Window Powershell`” from the `File` menu. 

2. You are now in your “terminal” that shows you’re inside the `RADiCAL_UELiveLinkClient` folder.

3. Type the following command: 
```.\RADiCAL_UELiveLinkClient.exe --serverUrl <serverUrl> --roomId <roomId> --authToken <authToken> --playerId <playerId> --poseName <live_link_pose_name>(optional)```

   Replace ```<serverUrl>``` you can get this from the `Room details` page as described above. Example: https://getrad.co/live/simulation-single/

   Replace ```<authToken>``` you can get this from you RADiCAL account settings: https://getrad.co/my-account/account-key-page/

   Replace ```<roomId>``` you can get this from the `Room details` page as described above. Example: https://getrad.co/live/simulation-single/

   Replace ```<playerId>``` you can get this from the 3D visualizer in our website, once you've connected to the room. Click the layers shaped button in the bottom bar to open editor, then click on a character. You will see the `playerId` value in the lover left corner, above the `roomId`.

   ```poseName``` is an optional parameter, use it if you want to name the LiveLink source.

   _Note: LiveLink source name has to be unique if you want to use more than one client at a time (if you want to create Sources for multiple charaters at the same time)_

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

## Multiplayer support (please go through previous steps first)

RADiCAL Live also supports multiplayer rooms, and we are working on improving the plug-in to make the experience integrated in the Unreal ecosystem. In the meantime we offer a more manual way of inputting our animation data in the Unreal Editor.

You can open the multiplayer simulation room here: https://getrad.co/live/simulation-multiplayer/. Follow the same steps as described before, and `Connect` to the simulation room. You should see 3 characters animated.

Now you should open two comand line windows (either Command Prompt or Windows PowerShell), and follow the steps described in `Enable the RADiCAL UE Live Link Client`, with only the `playerId` and `poseName` changed to reflect the ids from the simulation.

We now have LiveLink sources running in the background, time to play them in Unreal. In your Unreal Project, open the LiveLink window, and in `Sources` -> `Message Bus Sources` you should see the two RadicalLiveLink souces we created. Select both so you see them in the `source Type` list.

For every character you want to add its own animation source, you will have to create a duplicate of the main character blueprint and the animation blueprint corresponding to it. In our example (the sample Unreal project we provide), we will create a duplicate of the `BP_RADiCAL` character blueprint and the `AnimBP_RADiCAL_3-1` animation blueprint located in the `RADiCAL_Mannequin` folder. After creating the duplicate files, in our example `BP_RADiCAL_second` and `AnimBP_RADiCAL_3-1_2` double click on the new character blueprint, select the `SkeletalMesh` in the `Components` tab, and in the `Details` panel on the right, find the `Animation` tab, and select the duplicated animation in the `Anim Class` dropdown. (AnimBP_RADiCAL_3-1_2_C in our example)

![Char_blueprint](https://user-images.githubusercontent.com/7143949/144418700-73b5b65c-5cba-473d-95be-b42dd35eb139.png)

Next double click the Animation blueprint, go to the `AnimGraph` to the `Live Link Pose` node, and select from the dropdown one of the available sources. Make sure in the `Details` panel on the right, `Retarget Asset` dropdown has the `RadToRadRemapAsset` selected. You should now see the motion applied to your character.

![anim_blueprint](https://user-images.githubusercontent.com/7143949/144418738-678ba84f-efb4-4c88-8a30-139f56a9f3a9.png)


Open the other Animation blueprint and, in the `Live Link Pose` node, select the other source in the `Live Link Subject Name` dropdown. Now both characters will have their corresponding motion data applied to them.

Follow these simple steps to add as many characters and LiveLink sources as you want. 
 

## Rotation conversions for non-Epic skeletons (advanced users)

To account for some of the differences between the RADiCAL coordinate frame and Unreal, we have flipped the incoming LiveLink data's rotation and position axes. If you want to delve into the source C++, you can inspect the conversions at the `RadicalLiveLinkRemapAssetBase` class and its child classes. We expect that other skeletons will require different rotation adjustments, including swapping axes. We exposed three overridable methods to implement the root bone position, root bone rotation, and non-root bone rotation conversions. Please note that the AI output in RADiCAL uses the hip bone as a root, so position data should be mapped to the hips (or pelvis) in the target skeleton.
