CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 5;
	rq = 0;
	tid = 1224;
	title = 122402;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 7;
			elnk = 255;
			nextlnk = 101;
			prelnk = "4;";

			CDboTSActTWaitTS
			{
				taid = 1;
				time = 1000;
			}
		}
		CDboTSContGAct
		{
			cid = 8;
			elnk = 255;
			nextlnk = 102;
			prelnk = "4;";

			CDboTSActTWaitTS
			{
				taid = 1;
				time = 1000;
			}
		}
		CDboTSContGAct
		{
			cid = 6;
			elnk = 255;
			nextlnk = 100;
			prelnk = "4;";

			CDboTSActTWaitTS
			{
				taid = 1;
				time = 1000;
			}
		}
		CDboTSContGCond
		{
			cid = 105;
			prelnk = "102;";
			nolnk = 255;
			rm = 0;
			yeslnk = 102;

			CDboTSClickNPC
			{
				npcidx = "5591204;";
			}
		}
		CDboTSContGCond
		{
			cid = 104;
			prelnk = "101;";
			nolnk = 255;
			rm = 0;
			yeslnk = 101;

			CDboTSClickNPC
			{
				npcidx = "5591204;";
			}
		}
		CDboTSContGCond
		{
			cid = 103;
			prelnk = "100;";
			nolnk = 255;
			rm = 0;
			yeslnk = 100;

			CDboTSClickNPC
			{
				npcidx = "5591204;";
			}
		}
		CDboTSContGAct
		{
			cid = 3;
			elnk = 255;
			nextlnk = 4;
			prelnk = "2;";

			CDboTSActTWaitTS
			{
				taid = 1;
				time = 1000;
			}
		}
		CDboTSContGAct
		{
			cid = 2;
			elnk = 252;
			nextlnk = 3;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 122409;
				ctype = 1;
				idx = 5591204;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 122408;
				gtype = 1;
				area = 122401;
				goal = 122404;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 122405;
				stype = 1;
				taid = 1;
				title = 122402;
			}
		}
		CDboTSContUsrSel
		{
			cancellnk = 5;
			cid = 4;
			lilnk = 255;
			ndesc0 = 122421;
			uspt = -1;
			desc = 122420;
			nid2 = 8;
			type = 0;
			ust = 0;
			idx = 5591204;
			ndesc2 = 122423;
			nid1 = 7;
			prelnk = "3;5;";
			ndesc1 = 122422;
			nid0 = 6;
		}
		CDboTSContGCond
		{
			cid = 5;
			prelnk = "4;";
			nolnk = 255;
			rm = 0;
			yeslnk = 4;

			CDboTSClickNPC
			{
				npcidx = "5591204;";
			}
		}
		CDboTSContGAct
		{
			cid = 252;
			elnk = 255;
			nextlnk = 253;
			prelnk = "2;";
		}
		CDboTSContReward
		{
			canclnk = 104;
			cid = 101;
			rwdzeny = 0;
			desc = 122424;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 122403;
			rwdtype = 0;
			ltime = -1;
			prelnk = "7;104;";
			usetbl = 1;
		}
		CDboTSContReward
		{
			canclnk = 103;
			cid = 100;
			rwdzeny = 0;
			desc = 122424;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 122402;
			rwdtype = 0;
			ltime = -1;
			prelnk = "6;103;";
			usetbl = 1;
		}
		CDboTSContReward
		{
			canclnk = 105;
			cid = 102;
			rwdzeny = 0;
			desc = 122414;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 122401;
			rwdtype = 0;
			ltime = -1;
			prelnk = "8;105;";
			usetbl = 1;
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "0;1;252;";
			type = 0;
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "101;102;100;";
			type = 1;
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 122408;
			gtype = 1;
			oklnk = 2;
			area = 122401;
			goal = 122404;
			sort = 122405;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 122402;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 122407;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 41;
			}
			CDboTSCheckClrQst
			{
				flink = 1;
				not = 0;
			}
			CDboTSClickNPC
			{
				npcidx = "5591204;";
			}
		}
	}
}

