CREATE TABLE [Activations](
	[Id] [int] IDENTITY(1,1) NOT NULL,
	[ActivationKey] [nvarchar](256) NOT NULL,
	[LicenseKeyId] [int] NOT NULL,
	[HardwareId] [nvarchar](64) NOT NULL,
	[ExpirationDate] [datetime] NULL,
	[ActivationDate] [datetime] NOT NULL,
	[IPAddress] [nvarchar](45) NULL,
	CONSTRAINT [PK_Activations] PRIMARY KEY
(
	[Id]
))

CREATE INDEX IX_Activations_LicenseKeyId ON [Activations] ([LicenseKeyId])

CREATE TABLE [EventLog](
	[Id] [int] IDENTITY(1,1) NOT NULL,
	[Type] [tinyint] NOT NULL,
	[Subject] [nvarchar](200) NOT NULL,
	[Message] [nvarchar](4000) NULL,
	[Url] [nvarchar](4000) NULL,
	[UrlParams] [nvarchar](4000) NULL,
	[IPAddress] [nvarchar](45) NULL,
	[Location] [nvarchar](100) NULL,
	[Date] [datetime] NULL,
CONSTRAINT [PK_EventLog] PRIMARY KEY
(
	[Id]
))

CREATE TABLE [PaymentServiceMappings](
	[Id] [int] IDENTITY(1,1) NOT NULL,
	[ProductId] INT NOT NULL,
	[PaymentServiceType] [tinyint] NOT NULL,
	[PaymentServiceProductId] [nvarchar](50) NOT NULL,
	[PaymentServiceAccountId] [nvarchar](250) NULL,
CONSTRAINT [PK_PaymentServiceMappings] PRIMARY KEY
(
	[Id]
))

CREATE TABLE [Configuration](
	[Id] [int] IDENTITY(1,1) NOT NULL,
	[Name] [nvarchar](100) NOT NULL,
	[Value] [nvarchar](4000) NULL,
CONSTRAINT [PK_Configuration] PRIMARY KEY
(
	[Id]
),
CONSTRAINT [UC_Configuration_Name] UNIQUE 
(
	[Name]
))

CREATE TABLE [Downloads](
	[Id] [int] IDENTITY(1,1) NOT NULL,
	[Title] [nvarchar](200) NULL,
	[Description] [nvarchar](4000) NULL,
	[LocalPath] [nvarchar](200) NOT NULL,
	[Url] [nvarchar](4000) NULL,
	[MimeType] [nvarchar](100) NULL,
	[ProductId] [int] NULL,
	[NotificationEmail] [nvarchar](100) NULL,
	[IsPublic] [bit] NOT NULL DEFAULT 0,
	CONSTRAINT [PK_Downloads] PRIMARY KEY
	(
		[Id]
	),
	CONSTRAINT [UC_Downloads_LocalPath] UNIQUE 
	(
		[LocalPath]
	)
)

CREATE TABLE [LicenseKeys](
	[Id] [int] IDENTITY(1,1) NOT NULL,
	[Key] [nvarchar](500) NOT NULL,
	[ValidationData] [nvarchar](1000) NULL,
	[ProductId] [int] NOT NULL,
	[OrderItemId] [int] NULL,
	[DeviceLimit] [smallint] NULL,
	[DeviceCount] [int] NULL,
	[ActivationLimit] [smallint] NULL,
	[ActivationCount] [int] NULL,
	[LastActivationTime] [datetime] NULL,
	[PerDeviceActivationLimit] [smallint] NULL,
	[LicenseDuration] [smallint] NULL,
	[LicenseHistoryDuration] [smallint] NULL,
	[DateAdded] [datetime] NOT NULL DEFAULT GETDATE(),
	[Active] [bit] NOT NULL DEFAULT 1, 
	CONSTRAINT [PK_LicenseKeys] PRIMARY KEY
(
	[Id]
))

CREATE TABLE [LicenseKeysPool] (
	[Id] [int] NOT NULL,
	[ProductId] [int] NOT NULL,
	[LicenseKey] [nvarchar](500) NOT NULL,
CONSTRAINT [PK_LicenseKeysPool] PRIMARY KEY
(
	[Id]
))

CREATE TABLE [OrderItems](
	[Id] [int] IDENTITY(1,1) NOT NULL,
	[OrderId] [int] NOT NULL,
	[ProductId] [int] NOT NULL,
	[Quantity] [smallint] NOT NULL,
	[TotalPrice] [decimal](9, 2) NOT NULL,
	[TotalPriceExclFees] [decimal](9, 2) NULL,
	[OriginalProductPrice] [decimal](9, 2) NULL,
	[OriginalProductCurrency] [nvarchar](3) NULL,
CONSTRAINT [PK_OrderItems] PRIMARY KEY
(
	[Id]
))

CREATE TABLE [Orders](
	[Id] [int] IDENTITY(1,1) NOT NULL,
	[TransactionId] [nvarchar](50) NOT NULL DEFAULT NEWID(),
	[OrderTotal] [decimal](9, 2) NULL,
	[OrderTotalExclFees] [decimal](9, 2) NULL,
	[Currency] [nvarchar](3) NULL,
	[Date] [datetime] NOT NULL,
	[FirstName] [nvarchar](100) NULL,
	[LastName] [nvarchar](100) NULL,
	[Company] [nvarchar](100) NULL,
	[PhoneNumber] [nvarchar](50) NULL,
	[Email] [nvarchar](100) NULL,
	[Address] [nvarchar](200) NULL,
	[ZipCode] [nvarchar](50) NULL,
	[City] [nvarchar](50) NULL,
	[State] [nvarchar](50) NULL,
	[CountryCode] [nvarchar](50) NULL,
	CONSTRAINT [PK_Orders] PRIMARY KEY
	(
		[Id]
	),
	CONSTRAINT [UC_Orders_TransactionId] UNIQUE
	(
		[TransactionId]
	))

CREATE TABLE [Products](
	[Id] [int] NOT NULL,
	[Name] [nvarchar](100) NOT NULL,
	[Price] [decimal](9, 2) NULL,
	[Currency] [nvarchar](50) NULL,
	[DefaultLicenseDuration] [smallint] NULL,
	[DefaultMaxUniqueHardwareIdsPerLicenseKey] [smallint] NULL,
	[DefaultMaxActivationsPerHardwareId] [smallint] NULL,
	[DefaultMaxActivationsPerLicenseKey] [smallint] NULL,
	[LicenseKeyTemplate] [nvarchar](4000) NULL,
	[OrderEmailTemplate] [nvarchar](4000) NULL,
	[SupportEmail] [nvarchar](100) NULL,
	CONSTRAINT [PK_Products] PRIMARY KEY 
(
	[Id]
))

INSERT INTO Downloads(LocalPath, IsPublic) VALUES ('publicdownloadsample.txt', 1)

INSERT INTO Downloads(LocalPath, ProductId, IsPublic) VALUES ('restricteddownloadsample.txt', 12345, 0)

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

INSERT [LicenseKeys] ([ProductId], [Key],                           [DeviceLimit], [PerDeviceActivationLimit], [ActivationLimit], [LicenseDuration], [LicenseHistoryDuration]) 
VALUES                (12345,      N'X62DG-94SDT-A4TBZ-949CK-KMZB5', 0,            1,                          0,                 30,                180)

INSERT [LicenseKeys] ([ProductId], [Key],                           [DeviceLimit], [PerDeviceActivationLimit], [ActivationLimit], [LicenseDuration], [LicenseHistoryDuration]) 
VALUES                (54321,      N'AEAMW-FS7L5-2QSMC-AN4QH-8CMPS', 0,            1,                          0,                 30,                180)

INSERT [LicenseKeys] ([ProductId], [Key],                           [DeviceLimit], [PerDeviceActivationLimit], [ActivationLimit], [LicenseDuration], [LicenseHistoryDuration]) 
VALUES                (98765,      N'BWAAB-C6SM2-J9D6V-MDHRH-XX64U', 0,            1,                          0,                 30,                180)
