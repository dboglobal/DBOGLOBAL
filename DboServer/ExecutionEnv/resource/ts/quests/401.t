CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 2;
	rq = 0;
	tid = 401;
	title = 40102;

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
				tidx = 4371202;
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
				conv = 40109;
				ctype = 1;
				idx = 4371202;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 40108;
				area = 40101;
				goal = 40104;
				m0fz = "-4221.529785";
				m0widx = 1;
				scitem = -1;
				sort = 40105;
				stype = 64;
				m0fy = "0.000000";
				m0pat = 0;
				grade = 132203;
				m0fx = "6057.459961";
				m0ttip = 40115;
				rwd = 100;
				taid = 1;
				title = 40102;
				gtype = 1;
			}
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
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 40114;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 40101;
			rwdtype = 0;
			ltime = -1;
			prelnk = "4;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "4371202;";
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 40108;
			gtype = 1;
			oklnk = 2;
			area = 40101;
			goal = 40104;
			sort = 40105;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 40102;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 40107;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 1;
			}
			CDboTSCheckPCRace
			{
				raceflg = 4;
			}
			CDboTSCheckClrQst
			{
				flink = 1;
				flinknextqid = "402;";
				not = 0;
			}
			CDboTSClickNPC
			{
				npcidx = "4371202;";
			}
		}
	}
}

