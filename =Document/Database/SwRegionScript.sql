USE [RanGame1]
GO

/****** Object:  Table [dbo].[SwRegion]    Script Date: 09/03/2015 20:42:48 ******/
SET ANSI_NULLS ON
GO

SET QUOTED_IDENTIFIER ON
GO

CREATE TABLE [dbo].[SwRegion](
	[SwID] [int] NOT NULL,
	[SwNum] [int] NOT NULL,
 CONSTRAINT [PK_SwRegion] PRIMARY KEY CLUSTERED 
(
	[SwID] ASC,
	[SwNum] ASC
)WITH (PAD_INDEX  = OFF, STATISTICS_NORECOMPUTE  = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS  = ON, ALLOW_PAGE_LOCKS  = ON) ON [PRIMARY]
) ON [PRIMARY]

GO

ALTER TABLE [dbo].[SwRegion] ADD  CONSTRAINT [DF_SwRegion_SwNum]  DEFAULT ((3)) FOR [SwNum]
GO
