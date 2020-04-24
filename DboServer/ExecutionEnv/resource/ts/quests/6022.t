CNtlTSTrigger
{
	sm = 1;
	sq = 0;
	qc = -1;
	rq = 0;
	tid = 6022;
	title = 602202;

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
				conv = 602223;
				ctype = 1;
				idx = 3181301;
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
				clsflg = 512;
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
				conv = 602224;
				ctype = 1;
				idx = 3181304;
				taid = 1;
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
				npcidx = "3181301;";
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
				clsflg = 1024;
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
				conv = 602222;
				ctype = 1;
				idx = 3181301;
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
				conv = 602221;
				ctype = 1;
				idx = 3181301;
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
				id = 418;
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
				idx = 203;
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
				npcidx = "3181301;";
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 602207;
			nolnk = 254;
			rm = 0;
			yeslnk = 1;

			CDboTSClickNPC
			{
				npcidx = "3181301;";
			}
			CDboTSCheckClrQst
			{
				and = "6021;";
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
				conv = 602209;
				ctype = 1;
				idx = 3181301;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 602208;
				gtype = 2;
				area = 602201;
				goal = 602204;
				grade = -1;
				rwd = 100;
				scitem = -1;
				sort = 602205;
				stype = 8;
				taid = 1;
				title = 602202;
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
			desc = 602214;
			nextlnk = 253;
			rwdexp = 0;
			rwdtbl = 602201;
			rwdtype = 0;
			ltime = -1;
			prelnk = "12;";
			usetbl = 1;
		}
		CDboTSContProposal
		{
			cancellnk = 254;
			cid = 1;
			cont = 602208;
			gtype = 2;
			oklnk = 2;
			area = 602201;
			goal = 602204;
			sort = 602205;
			prelnk = "0;";
			ds = 1;
			grade = 0;
			rwd = 100;
			title = 602202;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 101;
			didx0 = -1;
			dval0 = 0;
			rwdzeny = 0;
			desc = 602220;
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
				npcidx = "3181304;";
			}
		}
	}
}

