CNtlTSTrigger
{
	sm = 1;
	sq = 0;
	qc = -1;
	rq = 0;
	tid = 6015;
	title = 601502;

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
				iidx1 = 99081;
				stype0 = 1;
				iidx0 = 99080;
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
			stdiag = 601507;
			nolnk = 255;
			rm = 0;
			yeslnk = 1;
		}
	}
	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 14;
			elnk = 255;
			nextlnk = 253;
			prelnk = "100;";

			CDboTSActWorldPlayScript
			{
				sid = 6015;
				start = 0;
				taid = 1;
				uof = 0;
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
			cid = 9;
			elnk = 254;
			nextlnk = 8;
			prelnk = "4;";

			CDboTSActWorldPlayScript
			{
				sid = 6015;
				start = 1;
				taid = 1;
				uof = 1;
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
				conv = 601525;
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
				conv = 601524;
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
				conv = 601523;
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
				conv = 601526;
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
				id = 197;
			}
		}
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
		CDboTSContStart
		{
			cid = 0;
			stdiag = 601507;
			nolnk = 254;
			rm = 0;
			yeslnk = 1;

			CDboTSClickNPC
			{
				npcidx = "3551101;";
			}
			CDboTSCheckClrQst
			{
				and = "6064;";
				flink = 0;
				not = 0;
			}
		}
		CDboTSContUsrSel
		{
			cancellnk = 3;
			cid = 4;
			lilnk = 255;
			ndesc0 = 601520;
			uspt = 3;
			desc = 601520;
			nid2 = 9;
			type = 0;
			ust = 1;
			idx = 1752104;
			ndesc2 = 601522;
			nid1 = 7;
			prelnk = "3;7;6;";
			ndesc1 = 601521;
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
				conv = 601509;
				ctype = 1;
				idx = 3551101;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 601508;
				gtype = 1;
				area = 601501;
				goal = 601504;
				grade = -1;
				rwd = 100;
				scitem = -1;
				sort = 601505;
				stype = 8;
				taid = 1;
				title = 601502;
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
			cont = 601508;
			gtype = 1;
			oklnk = 2;
			area = 601501;
			goal = 601504;
			sort = 601505;
			prelnk = "0;";
			ds = 1;
			grade = 0;
			rwd = 100;
			title = 601502;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 601514;
			nextlnk = 14;
			rwdexp = 0;
			rwdtbl = 601501;
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

