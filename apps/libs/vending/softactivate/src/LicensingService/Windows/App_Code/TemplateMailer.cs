using System;
using System.Collections.Generic;
using System.Web;
using System.Net.Mail;
using System.Threading;
using System.Net;
using System.IO;
using System.Collections;
using System.Text.RegularExpressions;
using System.Net.Mime;

public class TemplateMailer
{
    public TemplateMailer(string smtpSenderAddress, string smtpServer, int smtpPort, string smtpUsername, string smtpPassword, bool smtpSsl)
    {
        this.smtpSenderAddress = smtpSenderAddress;
        this.smtpServer = smtpServer;
        this.smtpPort = smtpPort;
        this.smtpUsername = smtpUsername;
        this.smtpPassword = smtpPassword;
        this.smtpSsl = smtpSsl;

        smtpClients = new Stack<SmtpClient>();
        smtpClientStackSem = new Semaphore(1, 1);

        for (int i = 0; i < 5; i++)
        {
            SmtpClient smtpClient = new SmtpClient(smtpServer, smtpPort);
            if (!(String.IsNullOrEmpty(smtpUsername) || String.IsNullOrEmpty(smtpPassword)))
            {
                smtpClient.UseDefaultCredentials = false;
                smtpClient.DeliveryMethod = SmtpDeliveryMethod.Network;
                smtpClient.Credentials = new NetworkCredential(smtpUsername, smtpPassword);
                smtpClient.EnableSsl = smtpSsl;
                smtpClient.Timeout = 5 * 60 * 1000;
            }

            smtpClients.Push(smtpClient);
        }
    }

    public delegate void SendEmailCaller(string recipient, string subject, string body, string bcc, string[] attachmentList, string[] linkedResourceList);

    public void SendEmail(string recipient, string subject, string body, string bcc = null, string[] attachmentList = null, string[] linkedResourceList = null)
    {
        SmtpClient smtpClient;

        smtpClientStackSem.WaitOne();

        lock (smtpClients)
        {
            smtpClient = smtpClients.Pop();
        }

        try
        {
            System.Text.UTF8Encoding utf8Encoder = new System.Text.UTF8Encoding();
            byte[] utf8EmailBody = utf8Encoder.GetBytes(body);
            MemoryStream emailBodyStream = new MemoryStream(utf8EmailBody, false);
            AlternateView htmlBody = new AlternateView(emailBodyStream, "text/html");

            if (linkedResourceList != null)
            {
                foreach (string resourceFile in linkedResourceList)
                {
                    LinkedResource resource = new LinkedResource(resourceFile);
                    resource.ContentId = Path.GetFileName(resourceFile);
                    resource.ContentType = new ContentType("image/png");
                    htmlBody.LinkedResources.Add(resource);
                }
            }

            MailMessage email = new MailMessage(smtpSenderAddress, recipient);
            email.Subject = subject;
            email.AlternateViews.Add(htmlBody);

            if (!String.IsNullOrEmpty(bcc))
                email.Bcc.Add(new MailAddress(bcc));

            // DEBUG
            // email.Bcc.Add("catalin.stavaru@gmail.com");

            if (attachmentList != null)
            {
                foreach (string attachmentFile in attachmentList)
                {
                    Attachment attachment = new Attachment(attachmentFile);
                    email.Attachments.Add(attachment);
                }
            }

            smtpClient.Send(email);
        }
        finally
        {
            lock (smtpClients)
            {
                smtpClients.Push(smtpClient);
            }

            smtpClientStackSem.Release();
        }
    }

    public object SendEmail(string template, Dictionary<string, string> parameters, string recipient, string bcc = null, string[] attachmentList = null, string[] linkedResourceList = null)
    {
        string subject;
        string body;

        BuildEmail(template, parameters, out subject, out body);

        SendEmail(recipient, subject, body, bcc, attachmentList, linkedResourceList);

        return null;
    }

    private void BuildEmail(string template, Dictionary<string, string> parameters, out string subject, out string body)
    {
        // ArrayList pathList;

        // Get linked resources
        /*
        Regex regex = new Regex("\"cid:([^\"]+)\"", RegexOptions.Multiline | RegexOptions.IgnoreCase);
        Match match = null;

        pathList = ((match = regex.Match(template)) != null) ? new ArrayList() : null;
        while (match.Groups.Count == 2)
        {
            pathList.Add(templateFolder + match.Groups[1].Value);
            match = match.NextMatch();
        }

        linkedResourceFiles = (string[])pathList.ToArray(typeof(string));
        */

        // Get attachments
        /*
        regex = new Regex("<!--##ATTACHMENT:([^#]+)##-->");

        pathList = ((match = regex.Match(template)) != null) ? new ArrayList() : null;
        while (match.Groups.Count == 2)
        {
            pathList.Add(templateFolder + match.Groups[1].Value);
            match = match.NextMatch();
        }

        attachmentFiles = (string[])pathList.ToArray(typeof(string));
        */

        // Replace parameters
        template = Regex.Replace(template, "##([^#]+)##",
                                 new MatchEvaluator(delegate(Match m)
                                 {
                                     string value;
                                     if (m.Groups.Count < 2) return m.Groups[0].Value;
                                     return (parameters.TryGetValue(m.Groups[1].Value, out value)) ? value : m.Groups[0].Value;
                                 }));

        // Get body
        body = template;

        // Get subject
        Regex regex = new Regex("<title>(.*)</title>");
        Match match = regex.Match(template);
        subject = match.Groups[1].Value;
    }

    private Dictionary<string, string> SplitToDictionary(string input)
    {
        Regex regex = new Regex(@"([^=]+)=([^=;]+)(;|$)");

        Dictionary<string, string> result = new Dictionary<string, string>();
        foreach (Match match in regex.Matches(input))
        {
            result.Add(match.Groups[1].Value.Trim(), match.Groups[2].Value.Trim());
        }

        return result;
    }

    Dictionary<string, string> templates = new Dictionary<string, string>();
    string smtpSenderAddress;
    string smtpServer;
    int smtpPort;
    string smtpUsername;
    string smtpPassword;
    bool smtpSsl;
    Stack<SmtpClient> smtpClients;
    Semaphore smtpClientStackSem;
}
