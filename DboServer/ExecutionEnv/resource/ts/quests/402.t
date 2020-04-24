CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 2;
	rq = 0;
	tid = 402;
	title = 40202;

	CNtlTSGroup
	{
		gid = 0;

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
			stdiag = 40207;
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
				minlvl = 1;
			}
			CDboTSClickNPC
			{
				npcidx = "4371202;";
			}
			CDboTSCheckClrQst
			{
				and = "401;";
				flink = 1;
				flinknextqid = "403;";
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
				conv = 40209;
				ctype = 1;
				idx = 4371202;
				taid = 2;
			}
			CDboTSActOpenWindow
			{
				show = 1;
				taid = 4;
				tblidx = 20002;
				wtype = 5;
			}
			CDboTSActSToCEvt
			{
				apptype = 0;
				cnt1 = 0;
				eitype = 0;
				idx0 = 7211100;
				cnt0 = 3;
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
				cont = 40208;
				area = 40201;
				goal = 40204;
				m0fz = "-4166.339844";
				m0widx = 1;
				scitem = -1;
				sort = 40205;
				stype = 2;
				m0fy = "0.000000";
				m0pat = 2;
				grade = 132203;
				m0fx = "6183.020020";
				m0ttip = 40215;
				rwd = 100;
				taid = 1;
				title = 40202;
				gtype = 2;
			}
		}
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
				eitype = 0;
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
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 40208;
			gtype = 2;
			oklnk = 2;
			area = 40201;
			goal = 40204;
			sort = 40205;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 40202;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 40214;
			nextlnk = 101;
			rwdexp = 0;
			rwdtbl = 40201;
			rwdtype = 0;
			ltime = -1;
			prelnk = "3;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "4371202;";
			}
		}
		CDboTSContGCond
		{
			cid = 3;
			prelnk = "2;";
			nolnk = 255;
			rm = 0;
			yeslnk = 100;

			CDboTSCheckSToCEvt
			{
				itype = 0;
			}
		}
	}
}

