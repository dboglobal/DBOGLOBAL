CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 8;
	rq = 0;
	tid = 2412;
	title = 241202;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 101;
			elnk = 255;
			nextlnk = 254;
			prelnk = "100;";

			CDboTSActSToCEvt
			{
				apptype = 0;
				cnt1 = 0;
				eitype = 1;
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
		CDboTSContStart
		{
			cid = 0;
			stdiag = 241207;
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
				npcidx = "5065103;";
			}
			CDboTSCheckClrQst
			{
				and = "2411;";
				flink = 1;
				flinknextqid = "2414;";
				not = 0;
			}
		}
		CDboTSContGAct
		{
			cid = 2;
			elnk = 252;
			nextlnk = 100;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 241209;
				ctype = 1;
				idx = 5065103;
				taid = 2;
			}
			CDboTSActSToCEvt
			{
				apptype = 0;
				cnt1 = 7;
				eitype = 1;
				idx0 = 1357;
				cnt0 = 5;
				cnt2 = 0;
				ectype = -1;
				etype = 0;
				idx1 = 1358;
				taid = 3;
				esctype = 0;
				idx2 = -1;
			}
			CDboTSActRegQInfo
			{
				cont = 241208;
				m3widx = 14;
				m1fy = "0.000000";
				sort = 241205;
				m2widx = 14;
				grade = 132203;
				m1pat = 2;
				m1widx = 14;
				m3fx = "855.000000";
				rwd = 100;
				taid = 1;
				gtype = 2;
				m3fy = "0.000000";
				area = 241201;
				goal = 241204;
				m0fz = "217.000000";
				m0widx = 14;
				m1ttip = 241215;
				m2fy = "0.000000";
				m2pat = 2;
				m3fz = "597.000000";
				m3ttip = 241216;
				scitem = -1;
				stype = 2;
				m0fy = "0.000000";
				m0pat = 2;
				m1fz = "484.000000";
				m2fx = "1010.000000";
				m2ttip = 241216;
				m0fx = "1136.000000";
				m0ttip = 241215;
				m3pat = 2;
				title = 241202;
				m1fx = "1150.000000";
				m2fz = "654.000000";
			}
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 241214;
			nextlnk = 101;
			rwdexp = 0;
			rwdtbl = 241201;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSCheckSToCEvt
			{
				itype = 1;
			}
			CDboTSClickNPC
			{
				npcidx = "5065103;";
			}
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "0;252;1;";
			type = 0;
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 241208;
			gtype = 2;
			oklnk = 2;
			area = 241201;
			goal = 241204;
			sort = 241205;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 241202;
		}
	}
	CNtlTSGroup
	{
		gid = 254;

		CDboTSContStart
		{
			cid = 0;
			stdiag = 241207;
			nolnk = 255;
			rm = 0;
			yeslnk = 1;
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "1;";
			type = 0;
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
				iidx1 = 1358;
				iprob0 = "1.000000";
				icnt1 = 255;
				iidx0 = 1357;
				iprob1 = "1.000000";
				taid = 1;
				type = 1;
			}
		}
	}
}

