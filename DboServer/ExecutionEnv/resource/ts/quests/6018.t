CNtlTSTrigger
{
	sm = 1;
	sq = 0;
	qc = -1;
	rq = 0;
	tid = 6018;
	title = 601802;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 14;
			elnk = 255;
			nextlnk = 6;
			prelnk = "101;";

			CDboTSActTWaitTS
			{
				taid = 1;
				time = 2000;
			}
		}
		CDboTSContGAct
		{
			cid = 13;
			elnk = 255;
			nextlnk = 253;
			prelnk = "12;";

			CDboTSActNPCConv
			{
				conv = 601823;
				ctype = 1;
				idx = 3332301;
				taid = 1;
			}
		}
		CDboTSContGAct
		{
			cid = 11;
			elnk = 255;
			nextlnk = 101;
			prelnk = "5;";

			CDboTSActNPCConv
			{
				conv = 601824;
				ctype = 1;
				idx = 3332303;
				taid = 1;
			}
		}
		CDboTSContGCond
		{
			cid = 12;
			prelnk = "9;";
			nolnk = 100;
			rm = 0;
			yeslnk = 13;

			CDboTSCheckPCCls
			{
				clsflg = 1024;
			}
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "13;10;100;";
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
				npcidx = "3332301;";
			}
		}
		CDboTSContGCond
		{
			cid = 9;
			prelnk = "8;";
			nolnk = 12;
			rm = 0;
			yeslnk = 10;

			CDboTSCheckPCCls
			{
				clsflg = 512;
			}
		}
		CDboTSContGAct
		{
			cid = 10;
			elnk = 255;
			nextlnk = 253;
			prelnk = "9;";

			CDboTSActNPCConv
			{
				conv = 601822;
				ctype = 1;
				idx = 3332301;
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
				conv = 601821;
				ctype = 1;
				idx = 3332301;
				taid = 1;
			}
		}
		CDboTSContGAct
		{
			cid = 6;
			elnk = 255;
			nextlnk = 7;
			prelnk = "14;";

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
			yeslnk = 11;

			CDboTSRcvSvrEvt
			{
				id = 419;
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
				idx = 204;
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
				npcidx = "3332301;";
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 601807;
			nolnk = 254;
			rm = 0;
			yeslnk = 1;

			CDboTSClickNPC
			{
				npcidx = "3332301;";
			}
			CDboTSCheckClrQst
			{
				and = "6017;";
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
				conv = 601809;
				ctype = 1;
				idx = 3332301;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 601808;
				gtype = 1;
				area = 601801;
				goal = 601804;
				grade = -1;
				rwd = 100;
				scitem = -1;
				sort = 601805;
				stype = 8;
				taid = 1;
				title = 601802;
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
			canclnk = 7;
			cid = 100;
			rwdzeny = 0;
			desc = 601814;
			nextlnk = 253;
			rwdexp = 0;
			rwdtbl = 601801;
			rwdtype = 0;
			ltime = -1;
			prelnk = "12;";
			usetbl = 1;
		}
		CDboTSContProposal
		{
			cancellnk = 254;
			cid = 1;
			cont = 601808;
			gtype = 1;
			oklnk = 2;
			area = 601801;
			goal = 601804;
			sort = 601805;
			prelnk = "0;";
			ds = 1;
			grade = 0;
			rwd = 100;
			title = 601802;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 101;
			didx0 = -1;
			dval0 = 0;
			rwdzeny = 0;
			desc = 601820;
			nextlnk = 14;
			rwdtbl = 600801;
			rwdtype = 0;
			dtype0 = 10;
			ltime = -1;
			prelnk = "11;";
			usetbl = 0;
			rwdexp = 0;

			CDboTSClickNPC
			{
				npcidx = "3332303;";
			}
		}
	}
}

