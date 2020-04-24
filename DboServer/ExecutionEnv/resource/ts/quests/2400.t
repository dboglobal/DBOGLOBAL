CNtlTSTrigger
{
	sm = 1;
	sq = 0;
	qc = 8;
	rq = 0;
	tid = 2400;
	title = 240002;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGCond
		{
			cid = 3;
			prelnk = "2;";
			nolnk = 255;
			rm = 0;
			yeslnk = 6;

			CDboTSRcvSvrEvt
			{
				id = 542;
			}
		}
		CDboTSContGAct
		{
			cid = 6;
			elnk = 255;
			nextlnk = 100;
			prelnk = "3;";

			CDboTSActPCConv
			{
				taid = 1;
				tblidx = 240023;
			}
		}
		CDboTSContGAct
		{
			cid = 101;
			elnk = 255;
			nextlnk = 4;
			prelnk = "100;";

			CDboTSActWorldPlayScript
			{
				sid = 2400;
				start = 0;
				taid = 1;
				uof = 0;
			}
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "4;5;";
			type = 1;
		}
		CDboTSContGCond
		{
			cid = 4;
			prelnk = "101;";
			nolnk = 254;
			rm = 0;
			yeslnk = 5;

			CDboTSCheckItem
			{
				icnt = 1;
				iidx = 98018;
			}
		}
		CDboTSContGAct
		{
			cid = 5;
			elnk = 255;
			nextlnk = 254;
			prelnk = "4;";

			CDboTSActItem
			{
				iidx0 = 98018;
				stype0 = 1;
				taid = 1;
				type = 1;
			}
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
			cid = 253;
			prelnk = "0;252;1;";
			type = 0;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 240007;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 52;
			}
			CDboTSClickNPC
			{
				npcidx = "6314103;";
			}
			CDboTSCheckClrQst
			{
				and = "2399;";
				flink = 1;
				flinknextqid = "2401;2439;";
				not = 0;
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
				conv = 240009;
				ctype = 1;
				idx = 6314103;
				taid = 2;
			}
			CDboTSActWorldPlayScript
			{
				sid = 2400;
				start = 1;
				taid = 3;
				uof = 1;
			}
			CDboTSActItem
			{
				iidx0 = 98018;
				stype0 = 1;
				taid = 4;
				type = 0;
			}
			CDboTSActRegQInfo
			{
				cont = 240008;
				m1fx = "731.101013";
				m2fz = "356.868988";
				m1fy = "-25.952000";
				sort = 240005;
				m2widx = 14;
				grade = 132203;
				m1pat = 0;
				m1widx = 14;
				rwd = 100;
				taid = 1;
				gtype = 2;
				area = 240001;
				goal = 240004;
				m0fz = "301.565002";
				m0widx = 14;
				m1ttip = 240016;
				m2fy = "-27.969999";
				m2pat = 0;
				scitem = 98018;
				stype = 2;
				m0fy = "3.704000";
				m0pat = 0;
				m1fz = "115.958000";
				m2fx = "588.429993";
				m2ttip = 240017;
				m0fx = "188.421997";
				m0ttip = 240015;
				title = 240002;
			}
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 240014;
			nextlnk = 101;
			rwdexp = 0;
			rwdtbl = 240001;
			rwdtype = 0;
			ltime = -1;
			prelnk = "6;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "6412111;";
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 240008;
			gtype = 2;
			oklnk = 2;
			area = 240001;
			goal = 240004;
			sort = 240005;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 240002;
		}
	}
	CNtlTSGroup
	{
		gid = 254;

		CDboTSContGAct
		{
			cid = 1;
			elnk = 255;
			nextlnk = 2;
			prelnk = "0;";

			CDboTSActItem
			{
				iidx0 = 98018;
				stype0 = 1;
				taid = 1;
				type = 1;
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 240007;
			nolnk = 255;
			rm = 0;
			yeslnk = 1;
		}
		CDboTSContEnd
		{
			cid = 2;
			prelnk = "1;";
			type = 0;
		}
	}
}

