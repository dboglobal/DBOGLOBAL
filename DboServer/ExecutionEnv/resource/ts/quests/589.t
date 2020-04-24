CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 2;
	rq = 0;
	tid = 589;
	title = 58902;

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
			cid = 2;
			elnk = 252;
			nextlnk = 100;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 58909;
				ctype = 1;
				idx = 2611202;
				taid = 2;
			}
			CDboTSActSToCEvt
			{
				apptype = 0;
				cnt1 = 0;
				eitype = 1;
				idx0 = 1067;
				cnt0 = 10;
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
				cont = 58908;
				area = 58901;
				goal = 58904;
				m0fz = "-1339.199951";
				m0widx = 1;
				scitem = -1;
				sort = 58905;
				stype = 1;
				m0fy = "0.000000";
				m0pat = 2;
				grade = 132203;
				m0fx = "7296.720215";
				m0ttip = 58915;
				rwd = 100;
				taid = 1;
				title = 58902;
				gtype = 1;
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 58908;
			gtype = 1;
			oklnk = 2;
			area = 58901;
			goal = 58904;
			sort = 58905;
			prelnk = "0;";
			ds = 1;
			grade = 0;
			rwd = 100;
			title = 58902;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 58914;
			nextlnk = 101;
			rwdexp = 0;
			rwdtbl = 58901;
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
				npcidx = "2611202;";
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 58907;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckPCRace
			{
				raceflg = 4;
			}
			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 19;
			}
			CDboTSClickNPC
			{
				npcidx = "2611202;";
			}
			CDboTSCheckClrQst
			{
				and = "588;";
				flink = 1;
				not = 0;
			}
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "252;0;1;";
			type = 0;
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "101;";
			type = 1;
		}
		CDboTSContGAct
		{
			cid = 252;
			elnk = 255;
			nextlnk = 253;
			prelnk = "2;";
		}
	}
	CNtlTSGroup
	{
		gid = 254;

		CDboTSContGAct
		{
			cid = 1;
			elnk = 255;
			nextlnk = 254;
			prelnk = "0;";

			CDboTSActQItem
			{
				icnt0 = 255;
				iprob0 = "1.000000";
				iidx0 = 1067;
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
			stdiag = 0;
			nolnk = 255;
			rm = 0;
			yeslnk = 1;
		}
	}
}

