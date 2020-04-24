CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 5;
	rq = 0;
	tid = 1393;
	title = 139302;

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
				conv = 139309;
				ctype = 1;
				idx = 8755301;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 139308;
				gtype = 1;
				area = 139301;
				goal = 139304;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 139305;
				stype = 2;
				taid = 1;
				title = 139302;
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 139307;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 44;
			}
			CDboTSClickNPC
			{
				npcidx = "8755301;";
			}
			CDboTSCheckClrQst
			{
				and = "1392;";
				flink = 1;
				flinknextqid = "1394;";
				not = 0;
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 139308;
			gtype = 1;
			oklnk = 2;
			area = 139301;
			goal = 139304;
			sort = 139305;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 139302;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 139314;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 139301;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "3171103;";
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
			prelnk = "100;";
			type = 1;
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "0;1;252;";
			type = 0;
		}
	}
}

