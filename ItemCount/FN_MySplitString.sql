USE [CHARACTER_01_DBF]
GO
/****** Object:  UserDefinedFunction [dbo].[my_split_string_sep]    Script Date: 02/05/2020 17:56:05 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
ALTER FUNCTION [dbo].[my_split_string_sep] ( @stringToSplit VARCHAR(MAX), @sep VARCHAR(MAX) )
RETURNS
 @returnList TABLE ([Name] [nvarchar] (500))
AS
BEGIN

 DECLARE @name NVARCHAR(255)
 DECLARE @pos INT

 WHILE CHARINDEX(@sep, @stringToSplit) > 0
 BEGIN
  SELECT @pos  = CHARINDEX(@sep, @stringToSplit)  
  SELECT @name = SUBSTRING(@stringToSplit, 1, @pos-1)

  INSERT INTO @returnList 
  SELECT @name

  SELECT @stringToSplit = SUBSTRING(@stringToSplit, @pos+1, LEN(@stringToSplit)-@pos)
 END

 INSERT INTO @returnList
 SELECT @stringToSplit

 RETURN
END