CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 0;
	rq = 0;
	tid = 1923;
	title = 192302;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 4;
			elnk = 255;
			nextlnk = 100;
			prelnk = "3;";

			CDboTSActOutMsg
			{
				idx = 14;
				taid = 1;
			}
		}
		CDboTSContGCond
		{
			cid = 3;
			prelnk = "2;";
			nolnk = 255;
			rm = 0;
			yeslnk = 4;

			CDboTSScoutUse
			{
				idx = 60000000;
				tidx = 3171106;
				tt = 1;
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
				conv = 192309;
				ctype = 1;
				idx = 3171106;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 192308;
				area = 192301;
				goal = 192304;
				m0fz = "4007.000000";
				m0widx = 1;
				scitem = -1;
				sort = 192305;
				stype = 64;
				m0fy = "0.000000";
				m0pat = 0;
				grade = 132203;
				m0fx = "4988.009766";
				m0ttip = 115;
				rwd = 100;
				taid = 1;
				title = 192302;
				gtype = 1;
			}
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 192314;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 192301;
			rwdtype = 0;
			ltime = -1;
			prelnk = "4;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "3171106;";
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 192307;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSClickNPC
			{
				npcidx = "3171106;";
			}
			CDboTSCheckPCRace
			{
				raceflg = 1;
			}
			CDboTSCheckPCCls
			{
				clsflg = 1;
			}
			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 1;
			}
			CDboTSCheckClrQst
			{
				flink = 1;
				flinknextqid = "1924;";
				not = 0;
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 192308;
			gtype = 1;
			oklnk = 2;
			area = 192301;
			goal = 192304;
			sort = 192305;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 192302;
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
			prelnk = "100;";
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
}

