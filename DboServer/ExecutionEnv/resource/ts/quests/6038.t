CNtlTSTrigger
{
	sm = 1;
	sq = 0;
	qc = -1;
	rq = 0;
	tid = 6038;
	title = 603802;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 15;
			elnk = 255;
			nextlnk = 6;
			prelnk = "101;";

			CDboTSActTWaitTS
			{
				taid = 1;
				time = 2000;
			}
		}
		CDboTSContGCond
		{
			cid = 13;
			prelnk = "9;";
			nolnk = 100;
			rm = 0;
			yeslnk = 14;

			CDboTSCheckPCCls
			{
				clsflg = 65536;
			}
		}
		CDboTSContGAct
		{
			cid = 14;
			elnk = 255;
			nextlnk = 253;
			prelnk = "13;";

			CDboTSActNPCConv
			{
				conv = 603822;
				ctype = 1;
				idx = 4751108;
				taid = 1;
			}
		}
		CDboTSContGAct
		{
			cid = 12;
			elnk = 255;
			nextlnk = 101;
			prelnk = "5;";

			CDboTSActNPCConv
			{
				conv = 603824;
				ctype = 1;
				idx = 4511515;
				taid = 2;
			}
			CDboTSActNPCConv
			{
				conv = 603824;
				ctype = 1;
				idx = 4751109;
				taid = 1;
			}
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "14;10;100;";
			type = 1;
		}
		CDboTSContGCond
		{
			cid = 8;
			prelnk = "7;";
			nolnk = 255;
			rm = 1;
			yeslnk = 9;

			CDboTSClickNPC
			{
				npcidx = "4751108;";
			}
		}
		CDboTSContGCond
		{
			cid = 9;
			prelnk = "8;";
			nolnk = 13;
			rm = 0;
			yeslnk = 10;

			CDboTSCheckPCCls
			{
				clsflg = 32768;
			}
		}
		CDboTSContProposal
		{
			cancellnk = 254;
			cid = 1;
			cont = 603808;
			gtype = 2;
			oklnk = 2;
			area = 603801;
			goal = 603804;
			sort = 603805;
			prelnk = "0;";
			ds = 1;
			grade = 0;
			rwd = 100;
			title = 603802;
		}
		CDboTSContReward
		{
			canclnk = 7;
			cid = 100;
			rwdzeny = 0;
			desc = 603814;
			nextlnk = 253;
			rwdexp = 0;
			rwdtbl = 603801;
			rwdtype = 0;
			ltime = -1;
			prelnk = "13;";
			usetbl = 1;
		}
		CDboTSContGAct
		{
			cid = 10;
			elnk = 255;
			nextlnk = 253;
			prelnk = "9;";

			CDboTSActNPCConv
			{
				conv = 603822;
				ctype = 1;
				idx = 4751108;
				taid = 1;
			}
		}
		CDboTSContGAct
		{
			cid = 7;
			elnk = 255;
			nextlnk = 8;
			prelnk = "6;100;";

			CDboTSActNPCConv
			{
				conv = 603821;
				ctype = 1;
				idx = 4751108;
				taid = 1;
			}
		}
		CDboTSContGAct
		{
			cid = 6;
			elnk = 255;
			nextlnk = 7;
			prelnk = "15;";

			CDboTSActTLQ
			{
				idx = 800001;
				taid = 1;
				type = 1;
			}
		}
		CDboTSContGCond
		{
			cid = 5;
			prelnk = "4;";
			nolnk = 255;
			rm = 0;
			yeslnk = 12;

			CDboTSRcvSvrEvt
			{
				id = 422;
			}
		}
		CDboTSContGAct
		{
			cid = 4;
			elnk = 255;
			nextlnk = 5;
			prelnk = "3;";

			CDboTSActTLQ
			{
				idx = 207;
				taid = 1;
				type = 0;
			}
		}
		CDboTSContGCond
		{
			cid = 3;
			prelnk = "2;";
			nolnk = 255;
			rm = 0;
			yeslnk = 4;

			CDboTSClickNPC
			{
				npcidx = "4751108;";
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 603807;
			nolnk = 254;
			rm = 0;
			yeslnk = 1;

			CDboTSClickNPC
			{
				npcidx = "4751108;";
			}
			CDboTSCheckClrQst
			{
				and = "6037;";
				flink = 0;
				not = 0;
			}
		}
		CDboTSContGAct
		{
			cid = 2;
			elnk = 255;
			nextlnk = 3;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 603809;
				ctype = 1;
				idx = 4751108;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 603808;
				gtype = 2;
				area = 603801;
				goal = 603804;
				grade = -1;
				rwd = 100;
				scitem = -1;
				sort = 603805;
				stype = 8;
				taid = 1;
				title = 603802;
			}
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "0;1;";
			type = 0;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 101;
			didx0 = -1;
			dval0 = 0;
			rwdzeny = 0;
			desc = 603820;
			nextlnk = 15;
			rwdtbl = 600801;
			rwdtype = 0;
			dtype0 = 10;
			ltime = -1;
			prelnk = "12;";
			usetbl = 0;
			rwdexp = 0;

			CDboTSClickNPC
			{
				npcidx = "4751109;";
			}
		}
	}
}

