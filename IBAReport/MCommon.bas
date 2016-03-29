Attribute VB_Name = "MCommon"
Option Explicit

Public Function AnsiToUnicode(ByVal strAnsi As String) As String
    AnsiToUnicode = StrConv(strAnsi, vbUnicode)
    AnsiToUnicode = TrimNull(AnsiToUnicode)
End Function

Public Function UnicodeToAnsi(ByVal strUnicode As String) As String
    UnicodeToAnsi = StrConv(IIf((Len(strUnicode) Mod 2) = 1, strUnicode & " ", strUnicode), vbFromUnicode)
End Function

Public Function TrimNull(ByVal Str As String) As String
    Dim pos As Long
    pos = InStr(1, Str, Chr(0))
    If pos > 0 Then Str = Left(Str, pos - 1)
    TrimNull = Trim$(Str)
End Function

Public Function GetStrPtr(ByVal Str As String) As Long
    GetStrPtr = StrPtr(UnicodeToAnsi(Str))
End Function

Public Function SaveFile(ByVal hwnd As Long, Optional ByVal dlgFilter As String = "", _
    Optional ByVal dlgInitDir As String = "", Optional ByVal dlgTitle As String = "����") As String
    On Error Resume Next
    Dim OFName As OPENFILENAME, sFileName As String
    sFileName = Space(cMaxPath)
    If dlgFilter = "" Then
        dlgFilter = "�����ļ�(*.*)" & vbNullChar & "*.*" & vbNullChar
    Else
        dlgFilter = Replace(dlgFilter, "|", vbNullChar)
        dlgFilter = dlgFilter & Chr(0) & "�����ļ�(*.*)" & vbNullChar & "*.*"
    End If
    
    dlgFilter = UnicodeToAnsi(dlgFilter)
    With OFName
        '���ýṹ�Ĵ�С
        .lStructSize = LenB(OFName)
        '���ø�����
        .hwndOwner = hwnd
        '���ó����ʵ��
        .hInstance = App.hInstance
        '���ù�������
        .nMaxCustFilter = cMaxPath
        .lpstrFilter = StrPtr(dlgFilter)
        '����Ĭ����չ��
        .lpstrFile = StrPtr(sFileName)
        '���÷��ص��ļ���ȫ·��������󳤶�
        .nMaxFile = cMaxPath
        'Ϊ�ļ����ƴ���������
        '.lpstrFileTitle = String(cMAXPATH, Chr(0))
        '���÷��ص��ļ����Ƶ���󳤶�
        .nMaxFileTitle = cMaxPath
        
        .lpstrDefExt = StrPtr("")
        
        If dlgInitDir <> "" Then
            .lpstrInitialDir = GetStrPtr(dlgInitDir & vbNullChar)
        End If
        '���öԻ������
        .lpstrTitle = GetStrPtr(dlgTitle)
        .flags = &H80000 Or &H2 Or &H1000 Or OFN_EXTENSIONDIFFERENT
    End With
    If GetSaveFileName(OFName) Then
        sFileName = AnsiToUnicode(sFileName)
        SaveFile = sFileName
    Else
        SaveFile = ""
    End If
End Function

