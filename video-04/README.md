# Video 04 - IPv6 Server on Azure VM
In this video:
- What is a Mapped IPv4 Address?
- How to deploy a IP4/IPv6 Azure VM.
- How to write Dual Stack Server code.
    
### dual stack IP4/IPv6 in Azure Virtual Network (VNET) with Std LB
[![Deploy To Azure](https://raw.githubusercontent.com/kirkster96/network-programming/main/images/deploytoazure.svg?sanitize=true)](https://portal.azure.com/#create/Microsoft.Template/uri/https%3A%2F%2Fraw.githubusercontent.com%2Fkirkster96%2Fnetwork-programming%2Fvideo04-dev%2Fvideo-04%2Fipv6-in-vnet-stdlb%2Fazuredeploy.json)

**This template demonstrates creation of a dual stack IPv4/IPv6 VNET with 1 IPv6 VM and a STANDARD Load Balancer.**
The template creates the following Azure resources:

- a dual stack IP4/IPv6 Virtual Network (VNET) with a dual stack subnet
- a virtual network interface (NIC) for each VM with both IPv4 and IPv6 endpoints
- an Internet-facing STANDARD Load Balancer with an IPv4 and an IPv6 Public IP addresses and health probe
- IPv6 Network Security Group rules (allow HTTP and RDP)
- an IPv6 User-Defined Route to a fictitious Network Virtual Appliance
- an IPv4 Public IP address for each VM to facilitate remote connection to the VM (RDP)
- two virtual machines with both IPv4 and IPv6 endpoints in the VNET/subnet

The template was originaly created from: [da0cdd9 on Jul 6,2022 azure-quickstart-templates/demos/ipv6-in-vnet-stdlb/](https://github.com/Azure/azure-quickstart-templates/tree/da0cdd93d7dd7d842c9c7e89738d4682c8013495/demos/ipv6-in-vnet-stdlb)
For more information about this template, see [What is IPv6 for Azure Virtual Network?](https://docs.microsoft.com/azure/virtual-network/ipv6-overview/)

[![Watch the video](https://img.youtube.com/vi/wiyZ7zhRzLM/hqdefault.jpg)](https://www.youtube.com/watch?v=wiyZ7zhRzLM)
