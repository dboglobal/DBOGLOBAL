CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 3;
	rq = 0;
	tid = 673;
	title = 67302;

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
				conv = 67309;
				ctype = 1;
				idx = 4371402;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 67308;
				gtype = 1;
				area = 67301;
				goal = 67304;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 67305;
				stype = 2;
				taid = 1;
				title = 67302;
			}
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 67314;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 67301;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "6312109;";
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
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 67308;
			gtype = 1;
			oklnk = 2;
			area = 67301;
			goal = 67304;
			sort = 67305;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 67302;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 67307;
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
				minlvl = 22;
			}
			CDboTSClickNPC
			{
				npcidx = "4371402;";
			}
			CDboTSCheckClrQst
			{
				and = "672;";
				flink = 1;
				not = 0;
			}
		}
	}
}

