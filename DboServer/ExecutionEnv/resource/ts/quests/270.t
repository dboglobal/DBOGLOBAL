CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 1;
	rq = 0;
	tid = 270;
	title = 27002;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 6;
			elnk = 255;
			nextlnk = 2;
			prelnk = "5;";

			CDboTSActItem
			{
				iidx0 = 99024;
				stype0 = 1;
				taid = 1;
				type = 0;
			}
		}
		CDboTSContGAct
		{
			cid = 5;
			elnk = 255;
			nextlnk = 6;
			prelnk = "4;";

			CDboTSActNPCConv
			{
				conv = 27002;
				ctype = 1;
				idx = 4751106;
				taid = 1;
			}
		}
		CDboTSContGCond
		{
			cid = 4;
			prelnk = "1;";
			nolnk = 5;
			rm = 0;
			yeslnk = 2;

			CDboTSCheckItem
			{
				icnt = 1;
				iidx = 99024;
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
			cid = 254;
			prelnk = "101;";
			type = 1;
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
			stdiag = 27007;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckPCRace
			{
				raceflg = 2;
			}
			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 12;
			}
			CDboTSClickNPC
			{
				npcidx = "4751106;";
			}
			CDboTSCheckClrQst
			{
				and = "265;";
				flink = 1;
				not = 0;
			}
		}
		CDboTSContGAct
		{
			cid = 2;
			elnk = 252;
			nextlnk = 100;
			prelnk = "6;4;";

			CDboTSActNPCConv
			{
				conv = 27009;
				ctype = 1;
				idx = 4751106;
				taid = 2;
			}
			CDboTSActSToCEvt
			{
				apptype = 0;
				cnt1 = 0;
				eitype = 3;
				idx0 = 475;
				cnt0 = 8;
				cnt2 = 0;
				ectype = -1;
				etype = 0;
				idx1 = -1;
				taid = 3;
				esctype = 0;
				idx2 = -1;
			}
			CDboTSActRegQInfo
			{
				cont = 27008;
				m1fx = "4539.279785";
				m1fy = "0.000000";
				sort = 27005;
				m0fy = "0.000000";
				m0pat = 2;
				m1fz = "-2706.489990";
				grade = 132203;
				m0fx = "4841.970215";
				m0ttip = 27015;
				m1pat = 2;
				m1widx = 1;
				rwd = 100;
				taid = 1;
				title = 27002;
				gtype = 2;
				area = 27001;
				goal = 27004;
				m0fz = "-2960.090088";
				m0widx = 1;
				m1ttip = 27015;
				scitem = -1;
				stype = 1;
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 27008;
			gtype = 2;
			oklnk = 4;
			area = 27001;
			goal = 27004;
			sort = 27005;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 27002;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 27014;
			nextlnk = 101;
			rwdexp = 0;
			rwdtbl = 27001;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSCheckSToCEvt
			{
				itype = 3;
			}
			CDboTSClickNPC
			{
				npcidx = "4751106;";
			}
		}
		CDboTSContGAct
		{
			cid = 101;
			elnk = 255;
			nextlnk = 254;
			prelnk = "100;";

			CDboTSActItem
			{
				iidx0 = 99024;
				stype0 = 1;
				taid = 2;
				type = 1;
			}
			CDboTSActSToCEvt
			{
				apptype = 0;
				cnt1 = 0;
				eitype = 3;
				idx0 = -1;
				cnt0 = 0;
				cnt2 = 0;
				ectype = -1;
				etype = 1;
				idx1 = -1;
				taid = 1;
				esctype = 0;
				idx2 = -1;
			}
		}
	}
	CNtlTSGroup
	{
		gid = 254;

		CDboTSContGAct
		{
			cid = 2;
			elnk = 255;
			nextlnk = 254;
			prelnk = "1;";

			CDboTSActQItem
			{
				icnt0 = 255;
				iprob0 = "1.000000";
				iidx0 = 475;
				taid = 1;
				type = 1;
			}
		}
		CDboTSContGCond
		{
			cid = 1;
			prelnk = "0;";
			nolnk = 254;
			rm = 0;
			yeslnk = 2;

			CDboTSCheckQItem
			{
				ct = 1;
				icnt = 1;
				iidx = 475;
			}
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "1;2;";
			type = 0;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 0;
			nolnk = 255;
			rm = 0;
			yeslnk = 1;
		}
	}
}

