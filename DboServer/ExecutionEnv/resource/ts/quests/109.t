CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 0;
	rq = 0;
	tid = 109;
	title = 10902;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 2;
			elnk = 252;
			nextlnk = 100;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 10909;
				ctype = 1;
				idx = 8032105;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 10908;
				gtype = 1;
				area = 10901;
				goal = 10904;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 10905;
				stype = 2;
				taid = 1;
				title = 10902;
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 10907;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckPCCls
			{
				clsflg = 3;
			}
			CDboTSCheckPCRace
			{
				raceflg = 1;
			}
			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 15;
			}
			CDboTSClickNPC
			{
				npcidx = "8032105;";
			}
			CDboTSCheckClrQst
			{
				flink = 1;
				flinknextqid = "110;";
				not = 0;
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
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 10908;
			gtype = 1;
			oklnk = 2;
			area = 10901;
			goal = 10904;
			sort = 10905;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 10902;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 10914;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 10901;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "3183104;";
			}
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

