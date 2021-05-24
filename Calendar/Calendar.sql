USE [CHARACTER_01_DBF]
GO

/****** Object:  Table [dbo].[CALENDAR_TBL]    Script Date: 11/05/2020 14:21:05 ******/
SET ANSI_NULLS ON
GO

SET QUOTED_IDENTIFIER ON
GO

SET ANSI_PADDING ON
GO

CREATE TABLE [dbo].[CALENDAR_TBL](
	[idplayer] [char](7) NULL,
	[serverindex] [char](2) NULL,
	[month] [int] NULL,
	[day] [int] NULL
) ON [PRIMARY]

GO

SET ANSI_PADDING OFF
GO


