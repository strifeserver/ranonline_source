USE [RanGame1]
GO

/****** Object:  Table [dbo].[VehicleInfo]    Script Date: 12/12/2013 15:28:27 ******/
SET ANSI_NULLS ON
GO

SET QUOTED_IDENTIFIER ON
GO

SET ANSI_PADDING ON
GO
/**Created By CNDEV**/
CREATE TABLE [dbo].[VehicleInfo](
	[VehicleUniqueNum] [int] IDENTITY(1,1) NOT NULL,
	[VehicleNum] [int] NOT NULL,
	[VehicleName] [varchar](20) NOT NULL,
	[VehicleChaNum] [int] NOT NULL,
	[VehicleType] [int] NOT NULL,
	[VehicleCardMID] [int] NOT NULL,
	[VehicleCardSID] [int] NOT NULL,
	[VehicleBattery] [int] NOT NULL,
	[VehicleDeleted] [int] NOT NULL,
	[VehicleCreateDate] [datetime] NOT NULL,
	[VehicleDeletedDate] [datetime] NOT NULL,
	[VehiclePutOnItems] [image] NOT NULL,
	[VehicleInfo] [int] NOT NULL,
	[VehicleColor] [int] NOT NULL,
	[VehicleColor1] [int] NOT NULL,
	[VehicleColor2] [int] NOT NULL,
	[VehicleColor3] [int] NOT NULL,
	[VehicleColor4] [int] NOT NULL,
	[VehicleColor5] [int] NOT NULL,
 CONSTRAINT [PK_VehicleInfo] PRIMARY KEY CLUSTERED 
(
	[VehicleUniqueNum] ASC
)WITH (PAD_INDEX  = OFF, STATISTICS_NORECOMPUTE  = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS  = ON, ALLOW_PAGE_LOCKS  = ON, FILLFACTOR = 90) ON [PRIMARY]
) ON [PRIMARY] TEXTIMAGE_ON [SECONDARY]

GO

SET ANSI_PADDING OFF
GO

ALTER TABLE [dbo].[VehicleInfo] ADD  CONSTRAINT [DF_VehicleInfo_VehicleNum]  DEFAULT ((0)) FOR [VehicleNum]
GO

ALTER TABLE [dbo].[VehicleInfo] ADD  CONSTRAINT [DF_VehicleInfo_VehicleName]  DEFAULT ('') FOR [VehicleName]
GO

ALTER TABLE [dbo].[VehicleInfo] ADD  CONSTRAINT [DF_VehicleInfo_VehicleChaNum]  DEFAULT ((0)) FOR [VehicleChaNum]
GO

ALTER TABLE [dbo].[VehicleInfo] ADD  CONSTRAINT [DF_VehicleInfo_VehicleType]  DEFAULT ((0)) FOR [VehicleType]
GO

ALTER TABLE [dbo].[VehicleInfo] ADD  CONSTRAINT [DF_VehicleInfo_VehicleCardMID]  DEFAULT ((0)) FOR [VehicleCardMID]
GO

ALTER TABLE [dbo].[VehicleInfo] ADD  CONSTRAINT [DF_VehicleInfo_VehicleCardSID]  DEFAULT ((0)) FOR [VehicleCardSID]
GO

ALTER TABLE [dbo].[VehicleInfo] ADD  CONSTRAINT [DF_VehicleInfo_VehicleBattery]  DEFAULT ((1000)) FOR [VehicleBattery]
GO

ALTER TABLE [dbo].[VehicleInfo] ADD  CONSTRAINT [DF_VehicleInfo_VehicleDeleted]  DEFAULT ((0)) FOR [VehicleDeleted]
GO

ALTER TABLE [dbo].[VehicleInfo] ADD  CONSTRAINT [DF_VehicleInfo_VehicleCreateDate]  DEFAULT (getdate()) FOR [VehicleCreateDate]
GO

ALTER TABLE [dbo].[VehicleInfo] ADD  CONSTRAINT [DF_VehicleInfo_VehicleDeletedDate]  DEFAULT (getdate()) FOR [VehicleDeletedDate]
GO

ALTER TABLE [dbo].[VehicleInfo] ADD  CONSTRAINT [DF_VehicleInfo_VehiclePutOnItems]  DEFAULT ('') FOR [VehiclePutOnItems]
GO

ALTER TABLE [dbo].[VehicleInfo] ADD  CONSTRAINT [DF_VehicleInfo_VehicleInfo]  DEFAULT ((0)) FOR [VehicleInfo]
GO

ALTER TABLE [dbo].[VehicleInfo] ADD  CONSTRAINT [DF_VehicleInfo_VehicleColor]  DEFAULT ((0)) FOR [VehicleColor]
GO

ALTER TABLE [dbo].[VehicleInfo] ADD  CONSTRAINT [DF_VehicleInfo_VehicleColor1]  DEFAULT ((0)) FOR [VehicleColor1]
GO

ALTER TABLE [dbo].[VehicleInfo] ADD  CONSTRAINT [DF_VehicleInfo_VehicleColor2]  DEFAULT ((0)) FOR [VehicleColor2]
GO

ALTER TABLE [dbo].[VehicleInfo] ADD  CONSTRAINT [DF_VehicleInfo_VehicleColor3]  DEFAULT ((0)) FOR [VehicleColor3]
GO

ALTER TABLE [dbo].[VehicleInfo] ADD  CONSTRAINT [DF_VehicleInfo_VehicleColor4]  DEFAULT ((0)) FOR [VehicleColor4]
GO

ALTER TABLE [dbo].[VehicleInfo] ADD  CONSTRAINT [DF_VehicleInfo_VehicleColor5]  DEFAULT ((0)) FOR [VehicleColor5]
GO