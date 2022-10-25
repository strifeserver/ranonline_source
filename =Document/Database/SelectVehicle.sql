USE [RanGame1]
GO

/****** Object:  StoredProcedure [dbo].[sp_SelectVehicle]    Script Date: 12/12/2013 15:28:10 ******/
SET ANSI_NULLS ON
GO

SET QUOTED_IDENTIFIER ON
GO


/**created by CNDEV**/
ALTER PROCEDURE [dbo].[sp_SelectVehicle]
	@nVehicleNum		int,
	@nVehicleChaNum		int
AS
	SET NOCOUNT ON
	
	SELECT VehicleChaNum, VehicleCardMID, VehicleCardSID,
	VehicleType, VehicleBattery, VehicleColor, VehicleColor1, VehicleColor2, VehicleColor3, VehicleColor4, VehicleColor5 FROM VehicleInfo WHERE VehicleNum = @nVehicleNum And VehicleChaNum = @nVehicleChaNum

	SET NOCOUNT OFF	

GO

