USE [RanGame1]
GO

/****** Object:  StoredProcedure [dbo].[sp_add_sw_region]    Script Date: 09/03/2015 20:43:28 ******/
SET ANSI_NULLS ON
GO

SET QUOTED_IDENTIFIER ON
GO





CREATE Procedure [dbo].[sp_add_sw_region]
	@SwID int,
    @SwNum    int,
	@nReturn int	OUTPUT	
AS
	DECLARE @error_var int, 
		    @rowcount_var int
		    
	SET NOCOUNT ON
		 
	SET @nReturn = 0	
	SET @error_var = 0
	SET @rowcount_var = 0
	
	IF EXISTS(SELECT * FROM SwRegion WHERE SwID=@SwID)
	BEGIN -- ??? ????
		UPDATE SwRegion 
		SET SwNum=@SwNum
		WHERE SwID=@SwID
		
		SELECT @error_var = @@ERROR, @rowcount_var = @@ROWCOUNT
		IF @error_var <> 0 OR @rowcount_var = 0 -- ????
		BEGIN
			SET @nReturn = 0
			SET NOCOUNT OFF
			RETURN @nReturn
		END
	END
	ELSE
	BEGIN
		INSERT INTO SwRegion (SwID, SwNum) 
		VALUES (@SwID, @SwNum)
		
		SELECT @error_var = @@ERROR, @rowcount_var = @@ROWCOUNT
		IF @error_var <> 0 OR @rowcount_var = 0 -- ????
		BEGIN
			SET @nReturn = 0
			SET NOCOUNT OFF
			RETURN @nReturn
		END
	END
	SET @nReturn = 1
	SET NOCOUNT OFF
	RETURN @nReturn




GO

