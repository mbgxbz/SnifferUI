#include "pcap.h"
#include "remote-ext.h"
#include "Afxtempl.h"
#include "Packet.h"

#define PCAP_ERRBUFF_SIZE	50

/* ���ļ��ļ��� */
char filename[50];

/* �ؼ�ָ�� */
CButton		*g_pBtnStart;
CButton		*g_pBtnPause;
CButton		*g_pBtnStop;
CComboBox	*g_pComboBoxDevList;
CListCtrl	*g_pListCtrlPacketList;
CTreeCtrl	*g_pTreeCtrlPacketInfo;
CEdit		*g_pEditCtrlPacketData;

/* ������Ϣ */
pcap_if_t *g_pAllDevs,*g_pDev;

/* pcapץ���õ��ı��� */
pcap_t *g_pAdhandle;

/* ȫ�ֱ���errbuf����Ŵ�����Ϣ */
char g_errbuf[PCAP_ERRBUF_SIZE];



/* ���ݰ��б����У���� */
int g_listctrlPacketListRows = -1;
int g_listctrlPacketListCols = 0;
int g_listctrlPacketListCount = 0;

/* ����������Packet��ʵ�� */
CList<Packet, Packet> g_packetLinkList;

/* �߳���ں��� */
UINT capture_thread(LPVOID pParam);

/* ���������� */
void packet_handler(u_char *dumpfile, const struct pcap_pkthdr *header, const u_char *pkt_data);

/* �ؼ���ʼ�� */
void initialComboBoxDevList();
void initialListCtrlPacketList();

/* ��ӡ */
int printListCtrlPacketList(const Packet &pkt);
int printListCtrlPacketList(const CList<Packet, Packet> &packetLinkList);


int	printEditCtrlPacketData(const Packet &pkt);

int printTreeCtrlPacketInfo(const Packet &pkt, int pktIndex);
int printEthernet2TreeCtrl(const Packet &pkt, HTREEITEM &parentNode);
int	printIP2TreeCtrl(const Packet &pkt, HTREEITEM &parentNode);
int	printARP2TreeCtrl(const Packet &pkt, HTREEITEM &parentNode);
int	printICMP2TreeCtrl(const Packet &pkt, HTREEITEM &parentNode);
int	printTCP2TreeCtrl(const Packet &pkt, HTREEITEM &parentNode);
int	printUDP2TreeCtrl(const Packet &pkt, HTREEITEM &parentNode);
int	printDNS2TreeCtrl(const Packet &pkt, HTREEITEM &parentNode);
int	printDHCP2TreeCtrl(const Packet &pkt, HTREEITEM &parentNode);
int	printHTTP2TreeCtrl(const Packet &pkt, HTREEITEM &parentNode);

/* ת�� */
CString	MACAddr2CString(const MAC_Address &addr);
CString	IPAddr2CString(const IP_Address &addr);

/* ����ת�� ���涨��ʽ��name2ת��Ϊ����name1 */
void translateName(char *name1, const char *name2);

/* DNS��Դ��¼���ݲ���ת�� ������ָ��c0�ĵ�ַdata2ת��Ϊ��ַdata1 offsetΪ��dns�ײ���ƫ����*/
void translateData(const DNS_Header *dnsh, char *data1, char *data2, const int data2_len);

/* �ж�data������ָ��0xc0,������ָ����data�е�λ��*/
int isNamePtr(char *data);