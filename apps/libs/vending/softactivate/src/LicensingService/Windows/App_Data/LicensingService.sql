
IF NOT EXISTS (SELECT 1 FROM sys.objects WHERE object_id = OBJECT_ID(N'[Activations]') and OBJECTPROPERTY(object_id, N'IsUserTable') = 1)
BEGIN
	CREATE TABLE [Activations](
		[Id] [int] IDENTITY(1,1) NOT NULL,
		[ActivationKey] [varchar](256) NOT NULL,
		[LicenseKeyId] [int] NOT NULL,
		[HardwareId] [varchar](64) NOT NULL,
		[ExpirationDate] [smalldatetime] NULL,
		[ActivationDate] [smalldatetime] NOT NULL,
		[IPAddress] [varchar](45) NULL,
	 CONSTRAINT [PK_Activations] PRIMARY KEY CLUSTERED 
	(
		[Id] ASC
	))
END
ELSE
BEGIN
    -- Id
	IF NOT EXISTS (SELECT 1 FROM sys.columns WHERE object_id=object_id('[Activations]') and NAME='Id')
		ALTER TABLE [Activations] ADD [Id] [int] IDENTITY(1, 1) NOT NULL

	-- ActivationKey
	IF NOT EXISTS (SELECT 1 FROM sys.columns WHERE object_id=object_id('[Activations]') and NAME='ActivationKey')
		ALTER TABLE [Activations] ADD [ActivationKey] [varchar](256) NOT NULL DEFAULT ''
	ELSE 
		ALTER TABLE [Activations] ALTER COLUMN [ActivationKey] [varchar](256) NOT NULL

	-- LicenseKeyId
	IF NOT EXISTS (SELECT 1 FROM sys.columns WHERE object_id=object_id('[Activations]') and NAME='LicenseKeyId')
		ALTER TABLE [Activations] ADD [LicenseKeyId] [int] NOT NULL DEFAULT 0
	ELSE
		ALTER TABLE [Activations] ALTER COLUMN [LicenseKeyId] [int] NOT NULL

	-- HardwareId
	IF NOT EXISTS (SELECT 1 FROM sys.columns WHERE object_id=object_id('[Activations]') and NAME='HardwareId')
		ALTER TABLE [Activations] ADD [HardwareId] [varchar](64) NOT NULL DEFAULT ''
	ELSE
		ALTER TABLE [Activations] ALTER COLUMN [HardwareId] [varchar](64) NOT NULL

	-- ExpirationDate
	IF NOT EXISTS (SELECT 1 FROM sys.columns WHERE object_id=object_id('[Activations]') and NAME='ExpirationDate')
		ALTER TABLE [Activations] ADD [ExpirationDate] [smalldatetime] NULL
	ELSE
		ALTER TABLE [Activations] ALTER COLUMN [ExpirationDate] [smalldatetime] NULL

	-- ActivationDate
	IF NOT EXISTS (SELECT 1 FROM sys.columns WHERE object_id=object_id('[Activations]') and NAME='ActivationDate')
		ALTER TABLE [Activations] ADD [ActivationDate] [smalldatetime] NOT NULL DEFAULT GETDATE()
	ELSE
		ALTER TABLE [Activations] ALTER COLUMN [ActivationDate] [smalldatetime] NOT NULL

	-- IPAddress
	IF NOT EXISTS (SELECT 1 FROM sys.columns WHERE object_id=object_id('[Activations]') and NAME='IPAddress')
		ALTER TABLE [Activations] ADD [IPAddress] [varchar](45) NULL
	ELSE
		ALTER TABLE [Activations] ALTER COLUMN [IPAddress] [varchar](45) NULL
END

GO

IF NOT EXISTS (SELECT 1 FROM sys.indexes WHERE NAME='IX_Activations_LicenseKeyId' AND object_id = OBJECT_ID(N'[Activations]'))
BEGIN
	CREATE INDEX IX_Activations_LicenseKeyId ON [Activations] ([LicenseKeyId])
END

GO

IF NOT EXISTS (SELECT 1 FROM sys.objects WHERE object_id = OBJECT_ID(N'[EventLog]') and OBJECTPROPERTY(object_id, N'IsUserTable') = 1)
BEGIN
	CREATE TABLE [EventLog](
		[Id] [int] IDENTITY(1,1) NOT NULL,
		[Type] [tinyint] NOT NULL,
		[Subject] [nvarchar](200) NOT NULL,
		[Message] [nvarchar](max) NULL,
		[Url] [nvarchar](max) NULL,
		[UrlParams] [nvarchar](max) NULL,
		[IPAddress] [varchar](45) NULL,
		[Location] [nvarchar](100) NULL,
		[Date] [smalldatetime] NULL,
	CONSTRAINT [PK_EventLog] PRIMARY KEY CLUSTERED 
	(
		[Id] ASC
	))
END

GO

IF NOT EXISTS (SELECT 1 FROM sys.objects WHERE object_id = OBJECT_ID(N'[PaymentServiceMappings]') and OBJECTPROPERTY(object_id, N'IsUserTable') = 1)
BEGIN
	CREATE TABLE [PaymentServiceMappings](
		[Id] [int] IDENTITY(1,1) NOT NULL,
		[ProductId] INT NOT NULL,
		[PaymentServiceType] [tinyint] NOT NULL,
		[PaymentServiceProductId] [varchar](50) NOT NULL,
		[PaymentServiceAccountId] [nvarchar](250) NULL,
	CONSTRAINT [PK_PaymentServiceMappings] PRIMARY KEY CLUSTERED 
	(
		[Id] ASC
	))
END
ELSE
BEGIN

    -- PaymentServiceAccountId
	IF NOT EXISTS (SELECT 1 FROM sys.columns WHERE object_id=object_id('[PaymentServiceMappings]') and NAME='PaymentServiceAccountId')
		ALTER TABLE [PaymentServiceMappings] ADD [PaymentServiceAccountId] [nvarchar] (250) NULL

END

GO

IF NOT EXISTS (SELECT 1 FROM sys.objects WHERE object_id = OBJECT_ID(N'[Configuration]') and OBJECTPROPERTY(object_id, N'IsUserTable') = 1)
BEGIN
	CREATE TABLE [Configuration](
		[Id] [int] IDENTITY(1,1) NOT NULL,
		[Name] [nvarchar](100) NOT NULL,
		[Value] [nvarchar](MAX) NULL,
	CONSTRAINT [PK_Configuration] PRIMARY KEY CLUSTERED 
	(
		[Id] ASC
	),
	CONSTRAINT [UC_Configuration_Name] UNIQUE 
	(
		[Name]
	))
END

GO

IF NOT EXISTS (SELECT 1 FROM sys.objects WHERE object_id = OBJECT_ID(N'[Downloads]') and OBJECTPROPERTY(object_id, N'IsUserTable') = 1)
BEGIN
	CREATE TABLE [Downloads](
		[Id] [int] IDENTITY(1,1) NOT NULL,
		[Title] [nvarchar](200) NULL,
		[Description] [nvarchar](MAX) NULL,
		[LocalPath] [nvarchar](200) NOT NULL,
		[Url] [nvarchar](MAX) NULL,
		[MimeType] [nvarchar](100) NULL,
		[ProductId] [int] NULL,
		[NotificationEmail] [nvarchar](100) NULL,
		[IsPublic] [bit] NOT NULL DEFAULT 0,
		CONSTRAINT [PK_Downloads] PRIMARY KEY CLUSTERED 
		(
			[Id] ASC
		),
		CONSTRAINT [UC_Downloads_LocalPath] UNIQUE 
		(
			[LocalPath]
		)
	)
END

GO

IF NOT EXISTS (SELECT 1 FROM sys.objects WHERE object_id = OBJECT_ID(N'[LicenseKeys]') and OBJECTPROPERTY(object_id, N'IsUserTable') = 1)
BEGIN
	CREATE TABLE [LicenseKeys](
		[Id] [int] IDENTITY(1,1) NOT NULL,
		[Key] [varchar](500) NOT NULL,
		[ValidationData] [varchar](1000) NULL,
		[ProductId] [int] NOT NULL,
		[OrderItemId] [int] NULL,
		[DeviceLimit] [smallint] NULL,
		[DeviceCount] [int] NULL,
		[ActivationLimit] [smallint] NULL,
		[ActivationCount] [int] NULL,
		[LastActivationTime] [smalldatetime] NULL,
		[PerDeviceActivationLimit] [smallint] NULL,
		[LicenseDuration] [smallint] NULL,
		[LicenseHistoryDuration] [smallint] NULL,
		[DateAdded] [smalldatetime] NOT NULL DEFAULT GETDATE(),
		[Active] [bit] NOT NULL DEFAULT 1,
		CONSTRAINT [PK_LicenseKeys] PRIMARY KEY CLUSTERED 
	(
		[Id] ASC
	))
END
ELSE
BEGIN
    -- Id
	IF NOT EXISTS (SELECT 1 FROM sys.columns WHERE object_id=object_id('[LicenseKeys]') and NAME='Id')
		ALTER TABLE [LicenseKeys] ADD [Id] [int] IDENTITY(1, 1) NOT NULL

	-- Key
	IF EXISTS (SELECT 1 FROM sys.columns WHERE object_id=object_id('[LicenseKeys]') and NAME='LicenseKey')
	BEGIN
		EXEC sp_rename 'LicenseKeys.LicenseKey', 'Key', 'COLUMN'
		ALTER TABLE [LicenseKeys] ALTER COLUMN [Key] [varchar](500) NOT NULL
	END
	ELSE
	BEGIN
		IF NOT EXISTS (SELECT 1 FROM sys.columns WHERE object_id=object_id('[LicenseKeys]') and NAME='Key')		
			ALTER TABLE [LicenseKeys] ADD [Key] [varchar](500) NOT NULL DEFAULT ''
		ELSE
			ALTER TABLE [LicenseKeys] ALTER COLUMN [Key] [varchar](500) NOT NULL
	END

	-- ValidationData
	IF NOT EXISTS (SELECT 1 FROM sys.columns WHERE object_id=object_id('[LicenseKeys]') and NAME='ValidationData')
		ALTER TABLE [LicenseKeys] ADD [ValidationData] [varchar](1000) NULL
	ELSE 
		ALTER TABLE [LicenseKeys] ALTER COLUMN [ValidationData] [varchar](1000) NULL

	-- ProductId
	IF NOT EXISTS (SELECT 1 FROM sys.columns WHERE object_id=object_id('[LicenseKeys]') and NAME='ProductId')
		ALTER TABLE [LicenseKeys] ADD [ProductId] [int] NOT NULL DEFAULT 0
	
	-- OrderItemId
	IF NOT EXISTS (SELECT 1 FROM sys.columns WHERE object_id=object_id('[LicenseKeys]') and NAME='OrderItemId')
		ALTER TABLE [LicenseKeys] ADD [OrderItemId] [int] NULL
	ELSE
		ALTER TABLE [LicenseKeys] ALTER COLUMN [OrderItemId] [int] NULL

	-- DeviceLimit
	IF EXISTS (SELECT 1 FROM sys.columns WHERE object_id=object_id('[LicenseKeys]') and NAME='MaxUniqueHardwareIds')
	BEGIN
		EXEC sp_rename 'LicenseKeys.MaxUniqueHardwareIds', 'DeviceLimit', 'COLUMN'
		ALTER TABLE [LicenseKeys] ALTER COLUMN [DeviceLimit] [smallint] NULL
	END
	ELSE 
	BEGIN
		IF NOT EXISTS (SELECT 1 FROM sys.columns WHERE object_id=object_id('[LicenseKeys]') and NAME='DeviceLimit')
			ALTER TABLE [LicenseKeys] ADD [DeviceLimit] [smallint] NULL
		ELSE
			ALTER TABLE [LicenseKeys] ALTER COLUMN [DeviceLimit] [smallint] NULL
	END

	-- DeviceCount
	IF NOT EXISTS (SELECT 1 FROM sys.columns WHERE object_id=object_id('[LicenseKeys]') and NAME='DeviceCount')
		ALTER TABLE [LicenseKeys] ADD [DeviceCount] [int] NULL
	ELSE
		ALTER TABLE [LicenseKeys] ALTER COLUMN [DeviceCount] [int] NULL

	-- ActivationLimit
	IF EXISTS (SELECT 1 FROM sys.columns WHERE object_id=object_id('[LicenseKeys]') and NAME='MaxActivations')
	BEGIN	
		EXEC sp_rename 'LicenseKeys.MaxActivations', 'ActivationLimit', 'COLUMN'
		ALTER TABLE [LicenseKeys] ALTER COLUMN [ActivationLimit] smallint NULL
	END
	ELSE
	BEGIN
		IF NOT EXISTS (SELECT 1 FROM sys.columns WHERE object_id=object_id('[LicenseKeys]') and NAME='ActivationLimit')
			ALTER TABLE [LicenseKeys] ADD [ActivationLimit] [smallint] NULL
		ELSE
			ALTER TABLE [LicenseKeys] ALTER COLUMN [ActivationLimit] [smallint] NULL
	END

	-- ActivationCount
	IF NOT EXISTS (SELECT 1 FROM sys.columns WHERE object_id=object_id('[LicenseKeys]') and NAME='ActivationCount')
		ALTER TABLE [LicenseKeys] ADD [ActivationCount] [int] NULL
	ELSE
		ALTER TABLE [LicenseKeys] ALTER COLUMN [ActivationCount] [int] NULL

	-- LastActivationTime
	IF NOT EXISTS (SELECT 1 FROM sys.columns WHERE object_id=object_id('[LicenseKeys]') and NAME='LastActivationTime')
		ALTER TABLE [LicenseKeys] ADD [LastActivationTime] [smalldatetime] NULL
	ELSE
		ALTER TABLE [LicenseKeys] ALTER COLUMN [LastActivationTime] [smalldatetime] NULL

	-- PerDeviceActivationLimit
	IF EXISTS (SELECT 1 FROM sys.columns WHERE object_id=object_id('[LicenseKeys]') and NAME='MaxActivationsPerHardwareId')
	BEGIN
		EXEC sp_rename 'LicenseKeys.MaxActivationsPerHardwareId', 'PerDeviceActivationLimit', 'COLUMN'
		ALTER TABLE [LicenseKeys] ALTER COLUMN [PerDeviceActivationLimit] [smallint] NULL
	END
	ELSE
	BEGIN
		IF NOT EXISTS (SELECT 1 FROM sys.columns WHERE object_id=object_id('[LicenseKeys]') and NAME='PerDeviceActivationLimit')
			ALTER TABLE [LicenseKeys] ADD [PerDeviceActivationLimit] [smallint] NULL
		ELSE
			ALTER TABLE [LicenseKeys] ALTER COLUMN [PerDeviceActivationLimit] [smallint] NULL
	END

	-- LicenseDuration
	IF NOT EXISTS (SELECT 1 FROM sys.columns WHERE object_id=object_id('[LicenseKeys]') and NAME='LicenseDuration')
		ALTER TABLE [LicenseKeys] ADD [LicenseDuration] [smallint] NULL
	ELSE
		ALTER TABLE [LicenseKeys] ALTER COLUMN [LicenseDuration] [smallint] NULL

	-- LicenseHistoryDuration
	IF NOT EXISTS (SELECT 1 FROM sys.columns WHERE object_id=object_id('[LicenseKeys]') and NAME='LicenseHistoryDuration')
		ALTER TABLE [LicenseKeys] ADD [LicenseHistoryDuration] [smallint] NULL
	ELSE
		ALTER TABLE [LicenseKeys] ALTER COLUMN [LicenseHistoryDuration] [smallint] NULL

	-- DateAdded
	IF NOT EXISTS (SELECT 1 FROM sys.columns WHERE object_id=object_id('[LicenseKeys]') and NAME='DateAdded')
		ALTER TABLE [LicenseKeys] ADD [DateAdded] [smalldatetime] NOT NULL DEFAULT GETUTCDATE()

	-- Active
	IF NOT EXISTS (SELECT 1 FROM sys.columns WHERE object_id=object_id('[LicenseKeys]') and NAME='Active')
		ALTER TABLE [LicenseKeys] ADD [Active] [bit] NOT NULL DEFAULT 1

END

GO

IF NOT EXISTS (SELECT 1 FROM sys.objects WHERE object_id = OBJECT_ID(N'[LicenseKeysPool]') and OBJECTPROPERTY(object_id, N'IsUserTable') = 1)
BEGIN
	CREATE TABLE [LicenseKeysPool] (
		[Id] [int] NOT NULL,
		[ProductId] [int] NOT NULL,
		[LicenseKey] [varchar](500) NOT NULL,
	CONSTRAINT [PK_LicenseKeysPool] PRIMARY KEY CLUSTERED 
	(
		[Id] ASC
	))
END

GO

IF NOT EXISTS (SELECT 1 FROM sys.objects WHERE object_id = OBJECT_ID(N'[OrderItems]') and OBJECTPROPERTY(object_id, N'IsUserTable') = 1)
BEGIN
	CREATE TABLE [OrderItems](
		[Id] [int] IDENTITY(1,1) NOT NULL,
		[OrderId] [int] NOT NULL,
		[ProductId] [int] NOT NULL,
		[Quantity] [smallint] NOT NULL,
		[TotalPrice] [decimal](9, 2) NOT NULL,
		[TotalPriceExclFees] [decimal](9, 2) NULL,
		[OriginalProductPrice] [decimal](9, 2) NULL,
		[OriginalProductCurrency] [varchar](3) NULL,
	CONSTRAINT [PK_OrderItems] PRIMARY KEY CLUSTERED 
	(
		[Id] ASC
	))
END

GO

IF NOT EXISTS (SELECT 1 FROM sys.objects WHERE object_id = OBJECT_ID(N'[Orders]') and OBJECTPROPERTY(object_id, N'IsUserTable') = 1)
BEGIN
	CREATE TABLE [Orders](
		[Id] [int] IDENTITY(1,1) NOT NULL,
		[TransactionId] [varchar](50) DEFAULT NEWID(),
		[OrderTotal] [decimal](9, 2) NULL,
		[OrderTotalExclFees] [decimal](9, 2) NULL,
		[Currency] [varchar](3) NULL,
		[Date] [smalldatetime] NOT NULL,
		[FirstName] [nvarchar](100) NULL,
		[LastName] [nvarchar](100) NULL,
		[Company] [nvarchar](100) NULL,
		[PhoneNumber] [varchar](50) NULL,
		[Email] [nvarchar](100) NULL,
		[Address] [nvarchar](200) NULL,
		[ZipCode] [nvarchar](50) NULL,
		[City] [nvarchar](50) NULL,
		[State] [nvarchar](50) NULL,
		[CountryCode] [varchar](50) NULL,
		CONSTRAINT [PK_Orders] PRIMARY KEY CLUSTERED 
		(
			[Id] ASC
		),
		CONSTRAINT [UC_Orders_TransactionId] UNIQUE
		(
			[TransactionId]
		))
END

ELSE

BEGIN
	IF NOT EXISTS (SELECT 1  FROM sys.objects WHERE object_id = OBJECT_ID(N'DF_Orders_TransactionId'))
	BEGIN	
		ALTER TABLE [Orders] ALTER COLUMN TransactionId VARCHAR(50) NULL

		UPDATE [Orders] SET [TransactionId] = NEWID() WHERE [TransactionId] IS NULL

		ALTER TABLE [Orders] ALTER COLUMN TransactionId VARCHAR(50) NOT NULL

		ALTER TABLE [Orders] ADD CONSTRAINT DF_Orders_TransactionId DEFAULT NEWID() FOR [TransactionId]	
	END
	
	;

	WITH cte AS (SELECT [TransactionId], [Date], ROW_NUMBER() OVER(PARTITION BY [TransactionId] ORDER BY [Date] DESC) 'RowRank' FROM [Orders])
		DELETE FROM cte WHERE RowRank > 1

    IF NOT EXISTS (SELECT 1  FROM sys.objects WHERE object_id = OBJECT_ID(N'UC_Orders_TransactionId'))
		ALTER TABLE [Orders] ADD CONSTRAINT [UC_Orders_TransactionId] UNIQUE ([TransactionId])
END

GO

IF NOT EXISTS (SELECT 1 FROM sys.objects WHERE object_id = OBJECT_ID(N'[Products]') and OBJECTPROPERTY(object_id, N'IsUserTable') = 1)
BEGIN
	CREATE TABLE [Products](
		[Id] [int] NOT NULL,
		[Name] [nvarchar](100) NOT NULL,
		[Price] [decimal](9, 2) NULL,
		[Currency] [varchar](50) NULL,
		[DefaultLicenseDuration] [smallint] NULL,
		[DefaultMaxUniqueHardwareIdsPerLicenseKey] [smallint] NULL,
		[DefaultMaxActivationsPerHardwareId] [smallint] NULL,
		[DefaultMaxActivationsPerLicenseKey] [smallint] NULL,
		[LicenseKeyTemplate] [nvarchar](max) NULL,
		[OrderEmailTemplate] [nvarchar](max) NULL,
		[SupportEmail] [nvarchar](100) NULL,
	 CONSTRAINT [PK_Products] PRIMARY KEY CLUSTERED 
	(
		[Id] ASC
	))
END
ELSE
BEGIN
    -- Id
	IF NOT EXISTS (SELECT 1 FROM sys.columns WHERE object_id=object_id('[Products]') and NAME='Id')
		ALTER TABLE [Products] ADD [Id] [int] NOT NULL DEFAULT 0
	ELSE
		ALTER TABLE [Products] ALTER COLUMN [Id] [int] NOT NULL	

	-- PayPalId
	IF NOT EXISTS (SELECT 1 FROM sys.columns WHERE object_id=object_id('[Products]') and NAME='PayPalId')
		ALTER TABLE [Products] ADD [PayPalId] [varchar](10) NULL
	ELSE
		ALTER TABLE [Products] ALTER COLUMN [PayPalId] [varchar](10) NULL		

	-- RegNowId
	IF NOT EXISTS (SELECT 1 FROM sys.columns WHERE object_id=object_id('[Products]') and NAME='RegNowId')
		ALTER TABLE [Products] ADD [RegNowId] [varchar](10) NULL
	ELSE
		ALTER TABLE [Products] ALTER COLUMN [RegNowId] [varchar](10) NULL	

	-- ShareItId
	IF NOT EXISTS (SELECT 1 FROM sys.columns WHERE object_id=object_id('[Products]') and NAME='ShareItId')
		ALTER TABLE [Products] ADD [ShareItId] [varchar](10) NULL
	ELSE
		ALTER TABLE [Products] ALTER COLUMN [ShareItId] [varchar](10) NULL

	-- Name
	IF NOT EXISTS (SELECT 1 FROM sys.columns WHERE object_id=object_id('[Products]') and NAME='Name')
		ALTER TABLE [Products] ADD [Name] [nvarchar](100) NOT NULL DEFAULT ''
	ELSE
		ALTER TABLE [Products] ALTER COLUMN [Name] [nvarchar](100) NOT NULL

	-- Price
	IF NOT EXISTS (SELECT 1 FROM sys.columns WHERE object_id=object_id('[Products]') and NAME='Price')
		ALTER TABLE [Products] ADD [Price] [decimal](9, 2) NULL
	ELSE
		ALTER TABLE [Products] ALTER COLUMN [Price] [decimal](9, 2) NULL

	-- Currency
	IF NOT EXISTS (SELECT 1 FROM sys.columns WHERE object_id=object_id('[Products]') and NAME='Currency')
		ALTER TABLE [Products] ADD [Currency] [varchar](50) NULL
	ELSE
		ALTER TABLE [Products] ALTER COLUMN [Currency] [varchar](50) NULL

	-- DefaultLicenseDuration
	IF NOT EXISTS (SELECT 1 FROM sys.columns WHERE object_id=object_id('[Products]') and NAME='DefaultLicenseDuration')
		ALTER TABLE [Products] ADD [DefaultLicenseDuration] [smallint] NULL	
	ELSE
		ALTER TABLE [Products] ALTER COLUMN [DefaultLicenseDuration] [smallint] NULL	

	-- DefaultMaxUniqueHardwareIdsPerLicenseKey
	IF NOT EXISTS (SELECT 1 FROM sys.columns WHERE object_id=object_id('[Products]') and NAME='DefaultMaxUniqueHardwareIdsPerLicenseKey')
		ALTER TABLE [Products] ADD [DefaultMaxUniqueHardwareIdsPerLicenseKey] [smallint] NULL	
	ELSE
		ALTER TABLE [Products] ALTER COLUMN [DefaultMaxUniqueHardwareIdsPerLicenseKey] [smallint] NULL	

	-- DefaultMaxActivationsPerHardwareId
	IF NOT EXISTS (SELECT 1 FROM sys.columns WHERE object_id=object_id('[Products]') and NAME='DefaultMaxActivationsPerHardwareId')
		ALTER TABLE [Products] ADD DefaultMaxActivationsPerHardwareId [smallint] NULL	
	ELSE
		ALTER TABLE [Products] ALTER COLUMN DefaultMaxActivationsPerHardwareId [smallint] NULL		
	
	-- DefaultMaxActivationsPerLicenseKey
	IF NOT EXISTS (SELECT 1 FROM sys.columns WHERE object_id=object_id('[Products]') and NAME='DefaultMaxActivationsPerLicenseKey')
		ALTER TABLE [Products] ADD [DefaultMaxActivationsPerLicenseKey] [smallint] NULL	
	ELSE
		ALTER TABLE [Products] ALTER COLUMN [DefaultMaxActivationsPerLicenseKey] [smallint] NULL	
	
	-- LicenseKeyTemplate
	IF NOT EXISTS (SELECT 1 FROM sys.columns WHERE object_id=object_id('[Products]') and NAME='LicenseKeyTemplate')
		ALTER TABLE [Products] ADD [LicenseKeyTemplate] [nvarchar](MAX) NULL	
	ELSE
		ALTER TABLE [Products] ALTER COLUMN [LicenseKeyTemplate] [nvarchar](MAX) NULL

	-- OrderEmailTemplate
	IF NOT EXISTS (SELECT 1 FROM sys.columns WHERE object_id=object_id('[Products]') and NAME='OrderEmailTemplate')
		ALTER TABLE [Products] ADD [OrderEmailTemplate] [nvarchar](MAX) NULL	
	ELSE
		ALTER TABLE [Products] ALTER COLUMN [OrderEmailTemplate] [nvarchar](MAX) NULL

	-- SupportEmail
	IF NOT EXISTS (SELECT 1 FROM sys.columns WHERE object_id=object_id('[Products]') and NAME='SupportEmail')
		ALTER TABLE [Products] ADD [SupportEmail] [nvarchar](100) NULL	
	ELSE
		ALTER TABLE [Products] ALTER COLUMN [SupportEmail] [nvarchar](100) NULL
END

GO

-- UPGRADE: move data from Purchases table into the Orders and OrderItems tables
IF EXISTS (SELECT 1 FROM sys.objects WHERE object_id = OBJECT_ID(N'[Purchases]') and OBJECTPROPERTY(object_id, N'IsUserTable') = 1)
BEGIN
	BEGIN TRAN
		DECLARE @Id int
		DECLARE @ProductId int
		DECLARE @TransactionId varchar(MAX)
		DECLARE @PurchaseDate smalldatetime
		DECLARE @LicenseKey varchar(MAX)
		DECLARE @LicenseExpirationDate smalldatetime
		DECLARE @CustomerName nvarchar(MAX)
		DECLARE @CustomerEmail nvarchar(MAX)
		DECLARE @CountryCode varchar(MAX)
		DECLARE @PaymentAmount DECIMAL(9, 2)
		DECLARE @PaymentCurrency varchar(MAX)

		-- move the license keys from the Purchases table into the LicenseKeys table, 
		-- and update the Order and OrderItems tables

		DECLARE cur_purchases CURSOR FOR
			SELECT 	[Id], [ProductId], [TransactionId],	[PurchaseDate], [LicenseKey], [LicenseExpirationDate], [CustomerName], [CustomerEmail], [CountryCode], [PaymentAmount], [PaymentCurrency] FROM [Purchases]

		OPEN cur_purchases
	
		FETCH NEXT FROM cur_purchases INTO @Id, @ProductId, @TransactionId, @PurchaseDate, @LicenseKey, @LicenseExpirationDate, @CustomerName, @CustomerEmail, @CountryCode, @PaymentAmount, @PaymentCurrency
		WHILE @@FETCH_STATUS = 0
		BEGIN
			BEGIN TRAN
				INSERT INTO [Orders] ([TransactionId], [OrderTotal],  [OrderTotalExclFees], [Currency],       [Date],        [FirstName],   [LastName],    [Email],        [CountryCode]) 
								   VALUES (@TransactionId, @PaymentAmount, @PaymentAmount,      @PaymentCurrency, @PurchaseDate, SUBSTRING(@CustomerName, 0, PATINDEX('%' + CHAR(32) +  '%', @CustomerName)), SUBSTRING(@CustomerName, PATINDEX('%' + CHAR(32) + '%', @CustomerName), 100), @CustomerEmail, @CountryCode)

				INSERT INTO [OrderItems] ([OrderId],        [ProductId], [Quantity], [TotalPrice],   [OriginalProductPrice], [OriginalProductCurrency]) 
								  VALUES (SCOPE_IDENTITY(), @ProductId,  1,          @PaymentAmount, @PaymentAmount,         @PaymentCurrency)

				INSERT INTO [LicenseKeys] ([Key],       [ProductId], [OrderItemId]) 
								   VALUES (@LicenseKey, @ProductId,  SCOPE_IDENTITY())
			COMMIT TRAN

			--fetch next identifier
			FETCH NEXT FROM cur_purchases INTO @Id, @ProductId, @TransactionId, @PurchaseDate, @LicenseKey, @LicenseExpirationDate, @CustomerName, @CustomerEmail, @CountryCode, @PaymentAmount, @PaymentCurrency
		END

		CLOSE cur_purchases

		DEALLOCATE cur_purchases

		DROP TABLE [Purchases]
		
	COMMIT TRAN
END

GO

-- move the license keys from Activations table into the LicenseKeys table
-- and update the Activations table with the resulting license key id

IF EXISTS (SELECT 1 FROM sys.columns WHERE object_id=object_id('[Activations]') and NAME='LicenseKey')
	AND EXISTS (SELECT 1 FROM sys.columns WHERE object_id=object_id('[Activations]') and NAME='LicenseKeyValidationData')
BEGIN
	DECLARE @sql NVARCHAR(MAX)

	SET @sql = '

	DECLARE @Id INT
	DECLARE @LicenseKeyId INT
	DECLARE @LicenseKey VARCHAR(MAX)
	DECLARE @LicenseKeyValidationData VARCHAR(MAX)

	DECLARE cur_activations CURSOR FOR
		SELECT 	[Id], [LicenseKey], [LicenseKeyValidationData] FROM [Activations]

	OPEN cur_activations

	FETCH NEXT FROM cur_activations INTO @Id, @LicenseKey, @LicenseKeyValidationData
	WHILE @@FETCH_STATUS = 0
	BEGIN
		SELECT @LicenseKeyId = [Id] FROM [LicenseKeys] WHERE [Key] = @LicenseKey
		IF @@ROWCOUNT > 0
			UPDATE [LicenseKeys] SET [ValidationData] = @LicenseKeyValidationData WHERE [Id] = @LicenseKeyId
		ELSE
		BEGIN
			INSERT INTO [LicenseKeys] ([Key], [ValidationData], [ProductId]) 
								VALUES (@LicenseKey,  @LicenseKeyValidationData, 0)

			UPDATE [Activations] SET [LicenseKeyId] = SCOPE_IDENTITY() WHERE [Id] = @Id
		END

		--fetch next identifier
		FETCH NEXT FROM cur_activations INTO @Id, @LicenseKey, @LicenseKeyValidationData
	END

	CLOSE cur_activations

	DEALLOCATE cur_activations

	IF EXISTS (SELECT 1 FROM sys.indexes WHERE object_id = object_id(''Activations'') AND NAME = ''IX_Activations_LicenseKey'')
		DROP INDEX [IX_Activations_LicenseKey] ON [Activations]

	ALTER TABLE [Activations] DROP COLUMN [LicenseKey]
	ALTER TABLE [Activations] DROP COLUMN [LicenseKeyValidationData]'

	EXEC sp_executesql @sql
END

GO

IF EXISTS (SELECT 1 FROM sys.columns WHERE object_id = object_id('[Products]') AND NAME = 'PayPalId')
AND EXISTS (SELECT 1 FROM sys.columns WHERE object_id = object_id('[Products]') AND NAME = 'RegNowId')
AND EXISTS (SELECT 1 FROM sys.columns WHERE object_id = object_id('[Products]') AND NAME = 'ShareItId')
BEGIN
	DECLARE @sql NVARCHAR(MAX)
	
	SELECT @sql = '

	DECLARE @Id INT
	DECLARE @PayPalId VARCHAR(20)
	DECLARE @ShareItId VARCHAR(20)
	DECLARE @RegNowId VARCHAR(20)

	DECLARE cur_products CURSOR FOR
		SELECT 	[Id], [PayPalId], [RegNowId], [ShareItId] FROM [Products]

	OPEN cur_products

	FETCH NEXT FROM cur_products INTO @Id, @PayPalId, @RegNowId, @ShareItId
	WHILE @@FETCH_STATUS = 0
	BEGIN
		IF @PayPalId IS NOT NULL
			IF NOT EXISTS (SELECT 1 FROM PaymentServiceMappings WHERE PaymentServiceType = 1 AND ProductId = @Id)
				INSERT INTO PaymentServiceMappings (PaymentServiceType, PaymentServiceProductId, ProductId) VALUES (1, @PayPalId, @Id)
			ELSE
				UPDATE PaymentServiceMappings SET PaymentServiceProductId = @PayPalId WHERE PaymentServiceType = 1 AND ProductId = @Id

		IF @RegNowId IS NOT NULL
			IF NOT EXISTS (SELECT 1 FROM PaymentServiceMappings WHERE PaymentServiceType = 2 AND ProductId = @Id)
				INSERT INTO PaymentServiceMappings (PaymentServiceType, PaymentServiceProductId, ProductId) VALUES (2, @RegNowId, @Id)
			ELSE
				UPDATE PaymentServiceMappings SET PaymentServiceProductId = @RegNowId WHERE PaymentServiceType = 2 AND ProductId = @Id

		IF @ShareItId IS NOT NULL
			IF NOT EXISTS (SELECT 1 FROM PaymentServiceMappings WHERE PaymentServiceType = 3 AND ProductId = @Id)
				INSERT INTO PaymentServiceMappings (PaymentServiceType, PaymentServiceProductId, ProductId) VALUES (3, @ShareItId, @Id)
			ELSE
				UPDATE PaymentServiceMappings SET PaymentServiceProductId = @ShareItId WHERE PaymentServiceType = 3 AND ProductId = @Id

		--fetch next identifier
		FETCH NEXT FROM cur_products INTO @Id, @PayPalId, @RegNowId, @ShareItId
	END

	CLOSE cur_products

	DEALLOCATE cur_products

	ALTER TABLE Products DROP COLUMN PayPalId
	ALTER TABLE Products DROP COLUMN RegNowId
	ALTER TABLE Products DROP COLUMN ShareItId'

	EXEC sp_executesql @sql
END

/*
IF NOT EXISTS (SELECT 1 FROM sys.objects WHERE object_id = OBJECT_ID(N'[UserProfile]') and OBJECTPROPERTY(object_id, N'IsUserTable') = 1)
BEGIN
	CREATE TABLE [UserProfile](
		[UserId] [int] IDENTITY(1,1) NOT NULL,
		[UserName] [nvarchar](56) NOT NULL,
	CONSTRAINT [PK_UserProfile] PRIMARY KEY CLUSTERED 
	(
		[UserId] ASC
	))
END

IF NOT EXISTS (SELECT 1 FROM sys.objects WHERE object_id = OBJECT_ID(N'[webpages_Membership]') and OBJECTPROPERTY(object_id, N'IsUserTable') = 1)
BEGIN
	CREATE TABLE [webpages_Membership](
		[UserId] [int] NOT NULL,
		[CreateDate] [datetime] NULL,
		[ConfirmationToken] [nvarchar](128) NULL,
		[IsConfirmed] [bit] NULL,
		[LastPasswordFailureDate] [datetime] NULL,
		[PasswordFailuresSinceLastSuccess] [int] NOT NULL,
		[Password] [nvarchar](128) NOT NULL,
		[PasswordChangedDate] [datetime] NULL,
		[PasswordSalt] [nvarchar](128) NOT NULL,
		[PasswordVerificationToken] [nvarchar](128) NULL,
		[PasswordVerificationTokenExpirationDate] [datetime] NULL,
	CONSTRAINT [PK_webpages_Membership] PRIMARY KEY CLUSTERED 
	(
		[UserId] ASC
	))
END

IF NOT EXISTS (SELECT 1 FROM sys.objects WHERE object_id = OBJECT_ID(N'[webpages_OAuthMembership]') and OBJECTPROPERTY(object_id, N'IsUserTable') = 1)
BEGIN
	CREATE TABLE [webpages_OAuthMembership](
		[Provider] [nvarchar](30) NOT NULL,
		[ProviderUserId] [nvarchar](100) NOT NULL,
		[UserId] [int] NOT NULL,
	CONSTRAINT [PK_webpages_OAuthMembership] PRIMARY KEY CLUSTERED 
	(
		[Provider] ASC,
		[ProviderUserId] ASC
	))
END

IF NOT EXISTS (SELECT 1 FROM sys.objects WHERE object_id = OBJECT_ID(N'[webpages_Roles]') and OBJECTPROPERTY(object_id, N'IsUserTable') = 1)
BEGIN
	CREATE TABLE [webpages_Roles](
		[RoleId] [int] IDENTITY(1,1) NOT NULL,
		[RoleName] [nvarchar](256) NOT NULL,
	PRIMARY KEY CLUSTERED 
	(
		[RoleId] ASC
	))
END

IF NOT EXISTS (SELECT 1 FROM sys.objects WHERE object_id = OBJECT_ID(N'[webpages_UsersInRoles]') and OBJECTPROPERTY(object_id, N'IsUserTable') = 1)
BEGIN
	CREATE TABLE [webpages_UsersInRoles](
		[UserId] [int] NOT NULL,
		[RoleId] [int] NOT NULL,
	PRIMARY KEY CLUSTERED 
	(
		[UserId] ASC,
		[RoleId] ASC
	))
END

ALTER TABLE [webpages_Membership] ADD  DEFAULT ((0)) FOR [IsConfirmed]

ALTER TABLE [webpages_Membership] ADD  DEFAULT ((0)) FOR [PasswordFailuresSinceLastSuccess]

ALTER TABLE [webpages_UsersInRoles]  WITH CHECK ADD  CONSTRAINT [fk_RoleId] FOREIGN KEY([RoleId]) REFERENCES [webpages_Roles] ([RoleId])

ALTER TABLE [webpages_UsersInRoles] CHECK CONSTRAINT [fk_RoleId]

ALTER TABLE [webpages_UsersInRoles]  WITH CHECK ADD  CONSTRAINT [fk_UserId] FOREIGN KEY([UserId]) REFERENCES [UserProfile] ([UserId])

ALTER TABLE [webpages_UsersInRoles] CHECK CONSTRAINT [fk_UserId]
*/

IF NOT EXISTS (SELECT 1 FROM [Downloads] WHERE [LocalPath] = 'publicdownloadsample.txt')
	INSERT INTO Downloads(LocalPath, IsPublic) VALUES ('publicdownloadsample.txt', 1)

IF NOT EXISTS (SELECT 1 FROM [Downloads] WHERE [LocalPath] = 'restricteddownloadsample.txt')
	INSERT INTO Downloads(LocalPath, ProductId, IsPublic) VALUES ('restricteddownloadsample.txt', 12345, 0)

IF NOT EXISTS (SELECT 1 FROM [Configuration] WHERE [Name] = 'DefaultOrderEmailTemplate')
	INSERT INTO [Configuration] (Name, Value) VALUES ('DefaultOrderEmailTemplate', 
	'<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
	<html xmlns="http://www.w3.org/1999/xhtml">
		<head>
			<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
			<title>Your ##PRODUCT_NAME## Order Information</title>
		</head>
		<body>
			<p>Dear Customer,</p>
        
			<p>Thank you for your purchase of ##PRODUCT_NAME##.</p>

			<p>Here is your license key: ##LICENSE_KEY##</p>

			<p>
				Thank you,<br />
				The Sales Team
			</p>
		</body>
	</html>')

GO

DECLARE @OrderEmailTemplate NVARCHAR(MAX)

SELECT @OrderEmailTemplate = Value FROM Configuration WHERE Name = 'DefaultOrderEmailTemplate'

IF NOT EXISTS (SELECT 1 FROM [Products] WHERE [Id] = 12345)
	INSERT [Products] ([Id],  [Name],            [Price],                     [Currency], [DefaultLicenseDuration], [DefaultMaxUniqueHardwareIdsPerLicenseKey], [DefaultMaxActivationsPerHardwareId], [DefaultMaxActivationsPerLicenseKey], [SupportEmail],           [LicenseKeyTemplate], [OrderEmailTemplate]) 
	           VALUES (12345, N'Sample Product 12345', CAST(0.01 AS Decimal(9, 2)), N'USD',     NULL,                     NULL,                                       NULL,                                 NULL,                                 N'support@mycompany.com', 
			   N'<?xml version=''1.0'' encoding=''utf-8''?>
<LicenseTemplate version=''3''>
	<LicenseKey encoding=''BASE32X'' characterGroups=''5'' charactersPerGroup=''5'' groupSeparator=''-'' header='''' footer=''''>
	<Data>
		<DataFields>
		<Field name=''ProductId'' type=''Integer'' size=''16'' offset=''0'' />
		</DataFields>
		<ValidationFields />
	</Data>
	<Signature size=''109''>
		<SignaturePublicKey>AI0bDQELdoxzyMNu7n46whOSAEon0PzzErF7AiD1r/HhLy4U1wA=</SignaturePublicKey>
		<SignaturePrivateKey>AQnD07PnEw7CRi8=</SignaturePrivateKey>
		<SigningServiceUrl>http://www.softactivate.com/SampleLicensingService/</SigningServiceUrl>
		<SigningServiceTemplateId>12345</SigningServiceTemplateId>
	</Signature>
	</LicenseKey>
	<Properties>
	<Trial>
		<TrialLicenseKey>X62DG-94SDT-A4TBZ-949CK-KMZB5</TrialLicenseKey>
	</Trial>
	</Properties>
</LicenseTemplate>',
				@OrderEmailTemplate)

IF NOT EXISTS (SELECT 1 FROM [Products] WHERE [Id] = 54321)
	INSERT [Products] ([Id],  [Name],            [Price],                     [Currency], [DefaultLicenseDuration], [DefaultMaxUniqueHardwareIdsPerLicenseKey], [DefaultMaxActivationsPerHardwareId], [DefaultMaxActivationsPerLicenseKey], [SupportEmail],           [LicenseKeyTemplate], [OrderEmailTemplate]) 
	           VALUES (54321, N'Sample Product 54321 w. ExpDate, RegName', CAST(0.01 AS Decimal(9, 2)), N'USD',     NULL,                     NULL,                                       NULL,                                 NULL,                                 N'support@mycompany.com', 
			   N'<?xml version=''1.0'' encoding=''utf-8''?>
<LicenseTemplate version=''3''>
	<LicenseKey encoding=''BASE32X'' characterGroups=''5'' charactersPerGroup=''5'' groupSeparator=''-'' header='''' footer=''''>
	<Data>
		<DataFields>
		<Field name=''ProductId'' type=''Integer'' size=''2'' offset=''0'' />
		<Field name=''ExpirationDate'' type=''Date14'' size=''14'' offset=''2'' />
		</DataFields>
		<ValidationFields>
		<Field name=''RegistrationName'' type=''String'' size=''800'' offset=''0'' />
		</ValidationFields>
	</Data>
	<Signature size=''109''>
		<SignaturePublicKey>AI0bDQELdoxzyMNu7n46whOSAH+azyCfw4Kx2FDADOO/01SjFgE=</SignaturePublicKey>
		<SignaturePrivateKey>AQnD07PnEw7CRi8=</SignaturePrivateKey>
		<SigningServiceUrl>http://www.softactivate.com/SampleLicensingService/</SigningServiceUrl>
		<SigningServiceTemplateId>54321</SigningServiceTemplateId>
	</Signature>
	</LicenseKey>
	<Properties>
	<Trial>
		<TrialKeyValidationData>VFJJQUwAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA==</TrialKeyValidationData>
		<TrialLicenseKey>AEAMW-FS7L5-2QSMC-AN4QH-8CMPS</TrialLicenseKey>
	</Trial>
	</Properties>
</LicenseTemplate>',
				@OrderEmailTemplate)

IF NOT EXISTS (SELECT 1 FROM [Products] WHERE [Id] = 98765)
	INSERT [Products] ([Id],  [Name],            [Price],                     [Currency], [DefaultLicenseDuration], [DefaultMaxUniqueHardwareIdsPerLicenseKey], [DefaultMaxActivationsPerHardwareId], [DefaultMaxActivationsPerLicenseKey], [SupportEmail],           [LicenseKeyTemplate], [OrderEmailTemplate]) 
	           VALUES (98765, N'Sample Product 98765 w. ExpDate, RegName, Features', CAST(0.01 AS Decimal(9, 2)), N'USD',     NULL,                     NULL,                                       NULL,                                 NULL,                                 N'support@mycompany.com', 
			   N'<?xml version=''1.0'' encoding=''utf-8''?>
<LicenseTemplate version=''3''>
  <LicenseKey encoding=''BASE32X'' characterGroups=''5'' charactersPerGroup=''5'' groupSeparator=''-'' header='''' footer=''''>
    <Data>
      <DataFields>
        <Field name=''ProductId'' type=''Integer'' size=''4'' offset=''0'' />
        <Field name=''FeatureBitMask'' type=''Integer'' size=''4'' offset=''4'' />
        <Field name=''ExpirationDate'' type=''Date14'' size=''14'' offset=''8'' />
      </DataFields>
      <ValidationFields>
        <Field name=''RegistrationName'' type=''String'' size=''800'' offset=''0'' />
      </ValidationFields>
    </Data>
    <Signature size=''103''>
      <SignaturePublicKey>AI0bCgAIZ8kMufl9HJySAEsTtjY7fXHDnbz9EV3XhIZE2wA=</SignaturePublicKey>
      <SignaturePrivateKey>AAdbPJfcWEw3</SignaturePrivateKey>
      <SigningServiceUrl>http://www.softactivate.com/SampleLicensingService/</SigningServiceUrl>
      <SigningServiceTemplateId>98765</SigningServiceTemplateId>
    </Signature>
  </LicenseKey>
  <Properties>
    <Trial>
      <TrialKeyValidationData>VFJJQUwAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA==</TrialKeyValidationData>
      <TrialLicenseKey>BWAAB-C6SM2-J9D6V-MDHRH-XX64U</TrialLicenseKey>
    </Trial>
  </Properties>
</LicenseTemplate>',
				@OrderEmailTemplate)

IF NOT EXISTS (SELECT 1 FROM LicenseKeys WHERE [Key] = 'X62DG-94SDT-A4TBZ-949CK-KMZB5')
	INSERT [LicenseKeys] ([ProductId], [Key],                           [DeviceLimit], [PerDeviceActivationLimit], [ActivationLimit], [LicenseDuration], [LicenseHistoryDuration]) 
	VALUES                (12345,      N'X62DG-94SDT-A4TBZ-949CK-KMZB5', 0,            1,                          0,                 30,                180)

IF NOT EXISTS (SELECT 1 FROM LicenseKeys WHERE [Key] = 'AEAMW-FS7L5-2QSMC-AN4QH-8CMPS')
	INSERT [LicenseKeys] ([ProductId], [Key],                           [DeviceLimit], [PerDeviceActivationLimit], [ActivationLimit], [LicenseDuration], [LicenseHistoryDuration]) 
	VALUES                (54321,      N'AEAMW-FS7L5-2QSMC-AN4QH-8CMPS', 0,            1,                          0,                 30,                180)

IF NOT EXISTS (SELECT 1 FROM LicenseKeys WHERE [Key] = 'BWAAB-C6SM2-J9D6V-MDHRH-XX64U')
	INSERT [LicenseKeys] ([ProductId], [Key],                           [DeviceLimit], [PerDeviceActivationLimit], [ActivationLimit], [LicenseDuration], [LicenseHistoryDuration]) 
	VALUES                (98765,      N'BWAAB-C6SM2-J9D6V-MDHRH-XX64U', 0,            1,                          0,                 30,                180)
