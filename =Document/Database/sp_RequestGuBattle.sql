set ANSI_NULLS ON
set QUOTED_IDENTIFIER ON
go




ALTER PROCEDURE [dbo].[sp_RequestGuBattle]
	@nGuPNum int,
	@nGuSNum int,
	@endTime datetime,
	@nAlliance int,
	@nReturn int OUTPUT
AS	
	DECLARE
		@error_var int, 
		@rowcount_var int
		
	SET NOCOUNT ON
	
	SET @nReturn = 0

    INSERT INTO GuildBattle (GuSNum, GuPNum, GuFlag, GuKillNum, GuDeathNum,
							 GuBattleStartDate, GuBattleEndDate, GuAlliance) 
    VALUES (@nGuSNum, @nGuPNum, 0, -- CLUB_BATTLE_NOW = 0
			0, 0, GETDATE(), @endTime, @nAlliance)
    
    SELECT @error_var = @@ERROR, @rowcount_var = @@ROWCOUNT    
    IF @error_var <> 0 OR @rowcount_var = 0
    BEGIN
        SET @nReturn = -1
        SET NOCOUNT OFF
        RETURN @nReturn
    END
    ELSE
    BEGIN
        SET @nReturn = 0
        SET NOCOUNT OFF
        RETURN @nReturn
    END



