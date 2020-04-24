CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 6;
	rq = 0;
	tid = 1740;
	title = 174002;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 11;
			elnk = 255;
			nextlnk = 8;
			prelnk = "5;";

			CDboTSActTWaitTS
			{
				taid = 1;
				time = 3000;
			}
		}
		CDboTSContGAct
		{
			cid = 12;
			elnk = 255;
			nextlnk = 9;
			prelnk = "6;";

			CDboTSActTWaitTS
			{
				taid = 1;
				time = 3000;
			}
		}
		CDboTSContGAct
		{
			cid = 10;
			elnk = 255;
			nextlnk = 7;
			prelnk = "4;";

			CDboTSActTWaitTS
			{
				taid = 1;
				time = 3000;
			}
		}
		CDboTSContGAct
		{
			cid = 8;
			elnk = 255;
			nextlnk = 101;
			prelnk = "11;";

			CDboTSActNPCConv
			{
				conv = 174025;
				ctype = 1;
				idx = 5063105;
				taid = 1;
			}
		}
		CDboTSContGAct
		{
			cid = 6;
			elnk = 255;
			nextlnk = 12;
			prelnk = "3;";

			CDboTSActNPCConv
			{
				conv = 174024;
				ctype = 1;
				idx = 5063105;
				taid = 1;
			}
		}
		CDboTSContGAct
		{
			cid = 5;
			elnk = 255;
			nextlnk = 11;
			prelnk = "3;";

			CDboTSActNPCConv
			{
				conv = 174024;
				ctype = 1;
				idx = 5063105;
				taid = 1;
			}
		}
		CDboTSContGAct
		{
			cid = 4;
			elnk = 255;
			nextlnk = 10;
			prelnk = "3;";

			CDboTSActNPCConv
			{
				conv = 174024;
				ctype = 1;
				idx = 5063105;
				taid = 1;
			}
		}
		CDboTSContGAct
		{
			cid = 9;
			elnk = 255;
			nextlnk = 100;
			prelnk = "12;";

			CDboTSActNPCConv
			{
				conv = 174026;
				ctype = 1;
				idx = 5063105;
				taid = 1;
			}
		}
		CDboTSContGAct
		{
			cid = 7;
			elnk = 255;
			nextlnk = 102;
			prelnk = "10;";

			CDboTSActNPCConv
			{
				conv = 174025;
				ctype = 1;
				idx = 5063105;
				taid = 1;
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 174007;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 49;
			}
			CDboTSClickNPC
			{
				npcidx = "5063105;";
			}
			CDboTSCheckClrQst
			{
				and = "1737;";
				flink = 1;
				not = 0;
			}
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "0;1;252;";
			type = 0;
		}
		CDboTSContUsrSel
		{
			cancellnk = 255;
			cid = 3;
			lilnk = 255;
			ndesc0 = 174021;
			uspt = -1;
			desc = 174020;
			nid2 = 6;
			type = 0;
			ust = 0;
			idx = 5063105;
			ndesc2 = 174023;
			nid1 = 5;
			prelnk = "2;";
			ndesc1 = 174022;
			nid0 = 4;
		}
		CDboTSContGAct
		{
			cid = 252;
			elnk = 255;
			nextlnk = 253;
			prelnk = "2;";
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "100;101;102;";
			type = 1;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 101;
			rwdzeny = 0;
			desc = 174014;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 174002;
			rwdtype = 0;
			ltime = -1;
			prelnk = "8;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "5063105;";
			}
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 102;
			rwdzeny = 0;
			desc = 174014;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 174003;
			rwdtype = 0;
			ltime = -1;
			prelnk = "7;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "5063105;";
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
				conv = 174009;
				ctype = 1;
				idx = 5063105;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 174008;
				gtype = 1;
				area = 174001;
				goal = 174004;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 174005;
				stype = 1;
				taid = 1;
				title = 174002;
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 174008;
			gtype = 1;
			oklnk = 2;
			area = 174001;
			goal = 174004;
			sort = 174005;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 174002;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 174014;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 174001;
			rwdtype = 0;
			ltime = -1;
			prelnk = "9;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "5063105;";
			}
		}
	}
}

