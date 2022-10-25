USE [RanGame1]
GO

/****** Object:  StoredProcedure [dbo].[sp_UpdateVehicleColor]    Script Date: 12/12/2013 15:27:46 ******/
SET ANSI_NULLS ON
GO

SET QUOTED_IDENTIFIER ON
GO



-- Return
-- DB_ERROR -1
-- DB_OK 0
/**Created By CNDEV**/
CREATE PROCEDURE [dbo].[sp_UpdateVehicleColor]
	@nVehicleNum	 int,
	@nCharNum	 int,
	@nVehicleColor int,
	@nVehicleColor1 int,
	@nVehicleColor2 int,
	@nVehicleColor3 int,
	@nVehicleColor4 int,
	@nVehicleColor5 int,
	@nReturn		int OUTPUT
AS	
	DECLARE
		@error_var int, 
		@rowcount_var int
		
	SET NOCOUNT ON
	
	SET @nReturn = 0	
	
	BEGIN TRAN

	UPDATE VehicleInfo
	SET VehicleColor=@nVehicleColor,VehicleColor1=@nVehicleColor1,VehicleColor2=@nVehicleColor2,VehicleColor3=@nVehicleColor3,VehicleColor4=@nVehicleColor4,VehicleColor5=@nVehicleColor5
	WHERE VehicleNum=@nVehicleNum AND VehicleChaNum = @nCharNum

	SELECT @error_var = @@ERROR, @rowcount_var = @@ROWCOUNT
	IF @error_var <> 0 OR @rowcount_var = 0
	BEGIN
		ROLLBACK TRAN
		SET @nReturn = -1
	END
	ELSE
	BEGIN
		COMMIT TRAN
		SET @nReturn = 0
	END    

	SET NOCOUNT OFF
	RETURN @nReturn


set ANSI_NULLS ON
set QUOTED_IDENTIFIER ON

GO

