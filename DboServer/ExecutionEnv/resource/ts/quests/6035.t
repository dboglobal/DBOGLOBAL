CNtlTSTrigger
{
	sm = 1;
	sq = 0;
	qc = -1;
	rq = 0;
	tid = 6035;
	title = 603502;

	CNtlTSGroup
	{
		gid = 254;

		CDboTSContGAct
		{
			cid = 1;
			elnk = 255;
			nextlnk = 254;
			prelnk = "0;";

			CDboTSActItem
			{
				iidx1 = 99085;
				stype0 = 1;
				iidx0 = 99084;
				stype1 = 1;
				taid = 1;
				type = 1;
			}
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "1;";
			type = 0;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 603507;
			nolnk = 255;
			rm = 0;
			yeslnk = 1;
		}
	}
	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGCond
		{
			cid = 3;
			prelnk = "2;4;";
			nolnk = 255;
			rm = 0;
			yeslnk = 4;

			CDboTSClickNPC
			{
				npcidx = "3551101;";
			}
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "14;";
			type = 1;
		}
		CDboTSContGAct
		{
			cid = 14;
			elnk = 255;
			nextlnk = 253;
			prelnk = "100;";

			CDboTSActWorldPlayScript
			{
				sid = 6035;
				start = 0;
				taid = 1;
				uof = 0;
			}
		}
		CDboTSContGAct
		{
			cid = 8;
			elnk = 255;
			nextlnk = 10;
			prelnk = "9;";

			CDboTSActNPCConv
			{
				conv = 603525;
				ctype = 1;
				idx = 3551101;
				taid = 1;
			}
		}
		CDboTSContGAct
		{
			cid = 7;
			elnk = 255;
			nextlnk = 4;
			prelnk = "4;";

			CDboTSActNPCConv
			{
				conv = 603524;
				ctype = 0;
				idx = 3551101;
				taid = 1;
			}
		}
		CDboTSContGAct
		{
			cid = 6;
			elnk = 255;
			nextlnk = 4;
			prelnk = "4;";

			CDboTSActNPCConv
			{
				conv = 603523;
				ctype = 0;
				idx = 3551101;
				taid = 1;
			}
		}
		CDboTSContGAct
		{
			cid = 12;
			elnk = 255;
			nextlnk = 100;
			prelnk = "10;";

			CDboTSActNPCConv
			{
				conv = 603526;
				ctype = 1;
				idx = 3551101;
				taid = 1;
			}
		}
		CDboTSContGCond
		{
			cid = 10;
			prelnk = "8;";
			nolnk = 255;
			rm = 0;
			yeslnk = 12;

			CDboTSRcvSvrEvt
			{
				id = 410;
			}
		}
		CDboTSContGAct
		{
			cid = 9;
			elnk = 254;
			nextlnk = 8;
			prelnk = "4;";

			CDboTSActWorldPlayScript
			{
				sid = 6035;
				start = 1;
				taid = 1;
				uof = 1;
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 603507;
			nolnk = 254;
			rm = 0;
			yeslnk = 1;

			CDboTSClickNPC
			{
				npcidx = "3551101;";
			}
			CDboTSCheckClrQst
			{
				and = "6066;";
				flink = 0;
				not = 0;
			}
		}
		CDboTSContUsrSel
		{
			cancellnk = 3;
			cid = 4;
			lilnk = 255;
			ndesc0 = 603520;
			uspt = 3;
			desc = 600520;
			nid2 = 9;
			type = 0;
			ust = 1;
			idx = 1752104;
			ndesc2 = 603522;
			nid1 = 7;
			prelnk = "3;7;6;";
			ndesc1 = 603521;
			nid0 = 6;
		}
		CDboTSContGAct
		{
			cid = 2;
			elnk = 255;
			nextlnk = 3;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 603509;
				ctype = 1;
				idx = 3551101;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 603508;
				gtype = 1;
				area = 603501;
				goal = 603504;
				grade = -1;
				rwd = 100;
				scitem = -1;
				sort = 603505;
				stype = 8;
				taid = 1;
				title = 603502;
			}
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "9;1;0;";
			type = 0;
		}
		CDboTSContProposal
		{
			cancellnk = 254;
			cid = 1;
			cont = 603508;
			gtype = 1;
			oklnk = 2;
			area = 603501;
			goal = 603504;
			sort = 603505;
			prelnk = "0;";
			ds = 1;
			grade = 0;
			rwd = 100;
			title = 603502;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 603514;
			nextlnk = 14;
			rwdexp = 0;
			rwdtbl = 603501;
			rwdtype = 0;
			ltime = -1;
			prelnk = "12;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "3551101;";
			}
		}
	}
}

