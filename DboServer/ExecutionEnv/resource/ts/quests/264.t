CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 1;
	rq = 0;
	tid = 264;
	title = 26402;

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
				iidx0 = 99023;
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
				conv = 26402;
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
				iidx = 99023;
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
			stdiag = 26407;
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
				minlvl = 11;
			}
			CDboTSClickNPC
			{
				npcidx = "4751106;";
			}
			CDboTSCheckClrQst
			{
				and = "263;";
				flink = 1;
				flinknextqid = "265;";
				not = 0;
			}
		}
		CDboTSContGAct
		{
			cid = 101;
			elnk = 255;
			nextlnk = 254;
			prelnk = "100;";

			CDboTSActQItem
			{
				icnt0 = 255;
				iprob0 = "1.000000";
				iidx0 = 513;
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
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 26414;
			nextlnk = 101;
			rwdexp = 0;
			rwdtbl = 26401;
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
			cid = 2;
			elnk = 252;
			nextlnk = 100;
			prelnk = "6;4;";

			CDboTSActNPCConv
			{
				conv = 26409;
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
				cnt0 = 1;
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
				cont = 26408;
				m1fx = "4539.279785";
				m1fy = "0.000000";
				sort = 26405;
				m0fy = "0.000000";
				m0pat = 2;
				m1fz = "-2706.489990";
				grade = 132203;
				m0fx = "4841.970215";
				m0ttip = 26415;
				m1pat = 2;
				m1widx = 1;
				rwd = 100;
				taid = 1;
				title = 26402;
				gtype = 2;
				area = 26401;
				goal = 26404;
				m0fz = "-2960.090088";
				m0widx = 1;
				m1ttip = 26415;
				scitem = -1;
				stype = 1;
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 26408;
			gtype = 2;
			oklnk = 4;
			area = 26401;
			goal = 26404;
			sort = 26405;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 26402;
		}
	}
	CNtlTSGroup
	{
		gid = 254;

		CDboTSContEnd
		{
			cid = 254;
			prelnk = "1;";
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
		CDboTSContGAct
		{
			cid = 1;
			elnk = 255;
			nextlnk = 254;
			prelnk = "0;";

			CDboTSActQItem
			{
				icnt0 = 255;
				iidx1 = 513;
				iprob0 = "1.000000";
				icnt1 = 255;
				iidx0 = 475;
				iprob1 = "1.000000";
				taid = 1;
				type = 1;
			}
		}
	}
}

